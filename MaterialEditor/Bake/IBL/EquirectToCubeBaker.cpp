#include "Bake/IBL/EquirectToCubeBaker.h"

EquirectToCubeBaker::EquirectToCubeBaker(std::weak_ptr<rcore::Window> const& window, std::string const& texturePath) : BakePass{ window }, m_texturePath{ texturePath }  {
  bake();
}

std::shared_ptr<rcore::RenderTarget> EquirectToCubeBaker::getEnvironmentCube() const {
  return m_envCube;
}

std::string EquirectToCubeBaker::makeCacheKey() const {
  std::error_code ec;
  auto size = std::filesystem::file_size(m_texturePath, ec);
  auto mtime = std::filesystem::last_write_time(m_texturePath, ec).time_since_epoch().count();
  if (ec) return {};

  std::string pathStr = std::filesystem::absolute(m_texturePath).generic_string();

  uint64_t h = hashData(pathStr.data(), pathStr.size());
  h = hashData(&size, sizeof(size), h);
  h = hashData(&mtime, sizeof(mtime), h);
  h = hashData(&m_textureWidth, sizeof(m_textureWidth), h);
  h = hashData(&m_textureHeight, sizeof(m_textureHeight), h);

  constexpr uint32_t bakerVersion = 1;
  h = hashData(&bakerVersion, sizeof(bakerVersion), h);

  return std::format("{:016x}", h);
}

std::vector<BakePass::CacheEntry> EquirectToCubeBaker::getCacheEntries() {
  return {
    CacheEntry{ "iblEnvironment_" + m_cacheKey + ".dds", &m_envCube }
  };
}

bool EquirectToCubeBaker::setupRenderTargets() {
  D3D11_TEXTURE2D_DESC textureDesc = rcore::Preset3D::makeCubeRenderTargetTextureDescription(m_textureWidth, m_textureHeight);
  textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  textureDesc.MipLevels = 0;

  auto srvDesc = rcore::Preset3D::makeCubeShaderResourceViewDescription();
  srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

  m_envCube = std::make_shared<rcore::RenderTarget>(textureDesc, srvDesc);

  return m_envCube->isValid();
}

bool EquirectToCubeBaker::bindSourceData() {
  bool result = true;

  result &= loadTexture();
  result &= makeMaterial();
  result &= makeFaceBuffer();

  return result;
}

bool EquirectToCubeBaker::draw() {
  static const FaceDirectionData faces[6] = {
    { {  1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0, -1 }, {} }, // +X
    { { -1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0,  1 }, {} }, // -X
    { {  0,  1,  0 }, {}, {  0,  0, -1 }, {}, {  1,  0,  0 }, {} }, // +Y
    { {  0, -1,  0 }, {}, {  0,  0,  1 }, {}, {  1,  0,  0 }, {} }, // -Y
    { {  0,  0,  1 }, {}, {  0,  1,  0 }, {}, {  1,  0,  0 }, {} }, // +Z
    { {  0,  0, -1 }, {}, {  0,  1,  0 }, {}, { -1,  0,  0 }, {} }, // -Z
  };

  D3D11_VIEWPORT viewport{};
  viewport.Width = static_cast<float>(m_textureWidth);
  viewport.Height = static_cast<float>(m_textureHeight);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  rcore::D3D11Device::get().rawContext()->RSSetViewports(1, &viewport);

  m_material->activate();

  for (int i = 0; i < 6; i++) {
    ID3D11RenderTargetView* rtv[] = { m_envCube->getRTV(i) };
    rcore::D3D11Device::get().rawContext()->OMSetRenderTargets(1, rtv, nullptr);

    m_faceBuffer->setData(faces[i]);
    if (!m_faceBuffer->uploadBuffer()) return false;

    rcore::D3D11Device::get().rawContext()->Draw(3, 0);
  }

  rcore::D3D11Device::get().rawContext()->GenerateMips(m_envCube->getSRV());

  return true;
}

bool EquirectToCubeBaker::loadTexture() {
  auto [hdrTexDesc, hdrSrvDesc] = rcore::Preset3D::makeHDRTextureDescriptionPair();

  m_texture = { rcore::LoaderTag<rcore::HDRLoader>{}, m_texturePath, hdrTexDesc, hdrSrvDesc };
  m_sampler = { rcore::Preset3D::makeStandardLinearSamplerDescription() };

  return m_texture.isValid() && m_sampler.isValid();
}

bool EquirectToCubeBaker::makeFaceBuffer() {
  m_faceBuffer = std::make_unique<rcore::CBuffer<FaceDirectionData>>(0, rcore::Pixel);
  return m_faceBuffer->isValid();
}

bool EquirectToCubeBaker::makeMaterial() {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc{ };

  rcore::Shader shader = { L"shaders/FullscreenTriVertexShader.hlsl", L"shaders/EquirectToCubePixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<EmptyMaterialProperties>>(shader, rcore::Pixel);

  if (!m_material->isValid())
    return false;

  ID3D11ShaderResourceView* texSrv[] = { m_texture.getTextureView() };
  m_material->setTextures(texSrv);

  ID3D11SamplerState* sState[] = { m_sampler.getSamplerState() };
  m_material->setSamplers(sState);

  return true;
}