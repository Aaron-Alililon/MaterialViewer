#include "Bake/IBL/BRDFLUTBaker.h"

BRDFLUTBaker::BRDFLUTBaker(std::weak_ptr<rcore::Window> const& window) : BakePass{ window } {
  bake();
}

std::shared_ptr<rcore::RenderTarget> BRDFLUTBaker::getLUT() const {
  return m_lut;
}

bool BRDFLUTBaker::setupRenderTargets() {
  D3D11_TEXTURE2D_DESC textureDesc = rcore::Preset3D::makeRenderTargetTextureDescription(m_size, m_size);
  textureDesc.Format = DXGI_FORMAT_R16G16_FLOAT;

  auto srvDesc = rcore::Preset3D::makeStandardShaderResourceViewDescription();
  srvDesc.Format = DXGI_FORMAT_R16G16_FLOAT;

  m_lut = std::make_shared<rcore::RenderTarget>(textureDesc, srvDesc);
  return m_lut->isValid();
}

bool BRDFLUTBaker::bindSourceData() {
  return makeMaterial();
}

bool BRDFLUTBaker::draw() {
  m_material->activate();

  D3D11_VIEWPORT viewport{};
  viewport.Width = static_cast<float>(m_size);
  viewport.Height = static_cast<float>(m_size);
  viewport.MaxDepth = 1.0f;
  rcore::D3D11Device::get().rawContext()->RSSetViewports(1, &viewport);

  ID3D11RenderTargetView* rtv[] = { m_lut->getRTV() };
  rcore::D3D11Device::get().rawContext()->OMSetRenderTargets(1, rtv, nullptr);

  rcore::D3D11Device::get().rawContext()->Draw(3, 0);
  return true;
}

bool BRDFLUTBaker::makeMaterial() {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc{ };

  rcore::Shader shader = { L"shaders/FullscreenTriVertexShader.hlsl", L"shaders/BRDFLUTPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<EmptyMaterialProperties>>(shader, rcore::Pixel);

  return m_material->isValid();
}