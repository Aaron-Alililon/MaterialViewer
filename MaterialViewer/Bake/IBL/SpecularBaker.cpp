#include "SpecularBaker.h"

SpecularBaker::SpecularBaker(std::weak_ptr<rcore::Window> const& window, std::shared_ptr<rcore::RenderTarget> const& envCube, std::string const& envCacheKey) : BakePass{ window }, m_envCube { envCube }, m_envCacheKey{ envCacheKey } {
  bake();
}

std::shared_ptr<rcore::RenderTarget> SpecularBaker::getPrefilteredCube() const {
  return m_prefilteredCube;
}

std::string SpecularBaker::makeCacheKey() const {
  uint64_t h;
  h = hashData(&m_textureWidth, sizeof(m_textureWidth));
  h = hashData(&m_textureHeight, sizeof(m_textureHeight), h);
  h = hashData(&m_mipCount, sizeof(m_mipCount), h);
  h = hashData(m_envCacheKey.data(), m_envCacheKey.size(), h);

  constexpr uint32_t bakerVersion = 1;
  h = hashData(&bakerVersion, sizeof(bakerVersion), h);

  return std::format("{:016x}", h);
}

std::vector<BakePass::CacheEntry> SpecularBaker::getCacheEntries() {
  return {
    CacheEntry{ "iblSpecular_" + m_cacheKey + ".dds", &m_prefilteredCube }
  };
}

bool SpecularBaker::setupRenderTargets() {
  D3D11_TEXTURE2D_DESC textureDesc = rcore::Preset3D::makeCubeRenderTargetTextureDescription(m_textureWidth, m_textureHeight);
  textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  textureDesc.MipLevels = m_mipCount;
  textureDesc.MiscFlags &= ~D3D11_RESOURCE_MISC_GENERATE_MIPS;

  auto srvDesc = rcore::Preset3D::makeCubeShaderResourceViewDescription();
  srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  srvDesc.TextureCube.MipLevels = m_mipCount;

  m_prefilteredCube = std::make_shared<rcore::RenderTarget>(textureDesc, srvDesc);

  return m_prefilteredCube->isValid();
}

bool SpecularBaker::bindSourceData() {
  bool result = true;

  result &= makeMaterial();
  result &= makeFaceBuffer();

  return result;
}

bool SpecularBaker::draw() {
  static const FaceDirectionData faces[6] = {
    { {  1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0, -1 }, 0 },
    { { -1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0,  1 }, 0 },
    { {  0,  1,  0 }, {}, {  0,  0, -1 }, {}, {  1,  0,  0 }, 0 },
    { {  0, -1,  0 }, {}, {  0,  0,  1 }, {}, {  1,  0,  0 }, 0 },
    { {  0,  0,  1 }, {}, {  0,  1,  0 }, {}, {  1,  0,  0 }, 0 },
    { {  0,  0, -1 }, {}, {  0,  1,  0 }, {}, { -1,  0,  0 }, 0 },
  };

  m_material->activate();

  for (UINT mip = 0; mip < m_mipCount; mip++) {
    UINT mipWidth = m_textureWidth >> mip;
    UINT mipHeight = m_textureHeight >> mip;
    float roughness = static_cast<float>(mip) / static_cast<float>(m_mipCount - 1);

    D3D11_VIEWPORT viewport{};
    viewport.Width = static_cast<float>(mipWidth);
    viewport.Height = static_cast<float>(mipHeight);
    viewport.MaxDepth = 1.0f;
    rcore::D3D11Device::get().rawContext()->RSSetViewports(1, &viewport);

    for (UINT face = 0; face < 6; face++) {
      ID3D11RenderTargetView* rtv[] = { m_prefilteredCube->getRTV(face, mip) };
      rcore::D3D11Device::get().rawContext()->OMSetRenderTargets(1, rtv, nullptr);

      FaceDirectionData data = faces[face];
      data.roughness = roughness;
      m_faceBuffer->setData(data);
      if (!m_faceBuffer->uploadBuffer()) return false;

      rcore::D3D11Device::get().rawContext()->Draw(3, 0);
    }
  }

  return true;
}

bool SpecularBaker::makeFaceBuffer() {
  m_faceBuffer = std::make_unique<rcore::CBuffer<FaceDirectionData>>(0, rcore::Pixel);
  return m_faceBuffer->isValid();
}

bool SpecularBaker::makeMaterial() {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc{ };

  rcore::Shader shader = { L"shaders/FullscreenTriVertexShader.hlsl", L"shaders/IBLSpecularPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<EmptyMaterialProperties>>(shader, rcore::Pixel);

  if (!m_material->isValid())
    return false;

  ID3D11ShaderResourceView* texSrv[] = { m_envCube->getSRV() };
  m_material->setTextures(texSrv);

  m_sampler = { rcore::Preset3D::makeStandardLinearSamplerDescription() };
  if (!m_sampler.isValid())
    return false;

  ID3D11SamplerState* sState[] = { m_sampler.getSamplerState() };
  m_material->setSamplers(sState);

  return true;
}