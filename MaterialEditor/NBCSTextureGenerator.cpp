#include "NBCSTextureGenerator.h"

NBCSTextureGenerator::NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<ModelSphere::ExtendedVertexType>> sivBuffer) :
    m_depthStencilState{ rcore::Preset3D::makeDisabledDepthStencilDescription() },
    m_rasterizerState{ rcore::Preset3D::makeNoCullingRasterDescription() }
{
  if (prepareTextures()) {
    bakeTangentBinormalTextures(window, sivBuffer);
  } else {
    RCORE_LOG(rcore::ERR, "Failed to prepare NBCS textures");
  }
}

std::pair<ID3D11ShaderResourceView*, ID3D11ShaderResourceView*> NBCSTextureGenerator::getSRVs() const {
  return std::make_pair(m_tangentTexture->getSRV(), m_binormalTexture->getSRV());
}

bool NBCSTextureGenerator::prepareTextures() {
  D3D11_TEXTURE2D_DESC textureDesc = rcore::Preset3D::makeRenderTargetTextureDescription(m_textureWidth, m_textureHeight);
  textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

  m_tangentTexture = std::make_unique<rcore::RenderTarget>(textureDesc);
  m_binormalTexture = std::make_unique<rcore::RenderTarget>(textureDesc);

  return m_tangentTexture->isValid() && m_binormalTexture->isValid();
}

std::shared_ptr<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>> NBCSTextureGenerator::material() const {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc = rcore::Preset3D::makeStandardInputDescription();
  D3D11_INPUT_ELEMENT_DESC sTangent{};
  sTangent.SemanticName = "TEXCOORD";
  sTangent.SemanticIndex = 1;
  sTangent.Format = DXGI_FORMAT_R32_FLOAT;
  sTangent.InputSlot = 0;
  sTangent.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  sTangent.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  sTangent.InstanceDataStepRate = 0;

  inputDesc.push_back(sTangent);

  D3D11_INPUT_ELEMENT_DESC sBinormal{};
  sBinormal.SemanticName = "TEXCOORD";
  sBinormal.SemanticIndex = 2;
  sBinormal.Format = DXGI_FORMAT_R32_FLOAT;
  sBinormal.InputSlot = 0;
  sBinormal.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  sBinormal.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  sBinormal.InstanceDataStepRate = 0;

  inputDesc.push_back(sBinormal);

  rcore::Shader shader = { L"NBCSTexVertexShader.hlsl", L"NBCSTexPixelShader.hlsl", inputDesc };
  return std::make_shared<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>>(shader, rcore::Pixel);
}

void NBCSTextureGenerator::bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<ModelSphere::ExtendedVertexType>> sivBuffer) {
  m_depthStencilState.bind();
  m_rasterizerState.bind();

  ID3D11RenderTargetView* rtvs[] = { m_tangentTexture->getRTV(), m_binormalTexture->getRTV() };
  rcore::D3D11Device::get().rawContext()->OMSetRenderTargets(2, rtvs, nullptr);

  D3D11_VIEWPORT bakeViewport{};
  bakeViewport.Width = static_cast<float>(m_textureWidth);
  bakeViewport.Height = static_cast<float>(m_textureHeight);
  bakeViewport.MinDepth = 0.0f;
  bakeViewport.MaxDepth = 1.0f;
  rcore::D3D11Device::get().rawContext()->RSSetViewports(1, &bakeViewport);

  m_tangentTexture->clearRTV();
  m_binormalTexture->clearRTV();

  rcore::Model model{ material(), sivBuffer.lock() };
  model.drawIndexed();

  auto lockedWindow = window.lock();
  if (!lockedWindow) { 
    RCORE_LOG(rcore::ERR, "Encountered invalid window pointer while generating tangent and binormal textures for NBCS");
    return;
  }

  lockedWindow->bindContextStates();
  lockedWindow->activateContext();
}