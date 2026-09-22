#include "IrradianceBaker.h"

IrradianceBaker::IrradianceBaker(std::weak_ptr<rcore::Window> const& window, std::shared_ptr<rcore::RenderTarget> const& envCube) : BakePass{ window }, m_envCube{ envCube } {
  bake();
}

std::shared_ptr<rcore::RenderTarget> IrradianceBaker::getIrradianceCube() const {
  return m_irradianceCube;
}

bool IrradianceBaker::setupRenderTargets() {
  D3D11_TEXTURE2D_DESC textureDesc = rcore::Preset3D::makeCubeRenderTargetTextureDescription(m_textureWidth, m_textureHeight);
  textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

  auto srvDesc = rcore::Preset3D::makeCubeShaderResourceViewDescription();
  srvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

  m_irradianceCube = std::make_shared<rcore::RenderTarget>(textureDesc, srvDesc);

  return m_irradianceCube->isValid();
}

bool IrradianceBaker::bindSourceData() {
  bool result = true;

  result &= makeMaterial();
  result &= makeFaceBuffer();

  return result;
}

bool IrradianceBaker::draw() {
  static const FaceDirectionData faces[6] = {
    { {  1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0, -1 }, {} }, // +X
    { { -1,  0,  0 }, {}, {  0,  1,  0 }, {}, {  0,  0,  1 }, {} }, // -X
    { {  0,  1,  0 }, {}, {  0,  0, -1 }, {}, {  1,  0,  0 }, {} }, // +Y
    { {  0, -1,  0 }, {}, {  0,  0,  1 }, {}, {  1,  0,  0 }, {} }, // -Y
    { {  0,  0,  1 }, {}, {  0,  1,  0 }, {}, {  1,  0,  0 }, {} }, // +Z
    { {  0,  0, -1 }, {}, {  0,  1,  0 }, {}, { -1,  0,  0 }, {} }, // -Z
  };

  m_material->activate();

  D3D11_VIEWPORT viewport{};
  viewport.Width = static_cast<float>(m_textureWidth);
  viewport.Height = static_cast<float>(m_textureHeight);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  rcore::D3D11Device::get().rawContext()->RSSetViewports(1, &viewport);

  for (int i = 0; i < 6; i++) {
    ID3D11RenderTargetView* rtv[] = { m_irradianceCube->getRTV(i) };
    rcore::D3D11Device::get().rawContext()->OMSetRenderTargets(1, rtv, nullptr);

    m_faceBuffer->setData(faces[i]);
    if (!m_faceBuffer->uploadBuffer()) return false;

    rcore::D3D11Device::get().rawContext()->Draw(3, 0);
  }

  return true;
}

bool IrradianceBaker::makeFaceBuffer() {
  m_faceBuffer = std::make_unique<rcore::CBuffer<FaceDirectionData>>(0, rcore::Pixel);
  return m_faceBuffer->isValid();
}

bool IrradianceBaker::makeMaterial() {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc{ };

  rcore::Shader shader = { L"shaders/FullscreenTriVertexShader.hlsl", L"shaders/IBLIrradiancePixelShader.hlsl", inputDesc };
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