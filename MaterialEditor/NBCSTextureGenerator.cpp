#include "NBCSTextureGenerator.h"

NBCSTextureGenerator::NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer) :
    m_depthStencilState{ rcore::Preset3D::makeDisabledDepthStencilDescription() },
    m_rasterizerState{ rcore::Preset3D::makeNoCullingRasterDescription() }
{
  if (prepareTextures()) {
    bakeTangentBinormalTextures(window, makeExtendedSIVBuffer(sivBuffer));
  } else {
    RCORE_LOG(rcore::ERR, "Failed to prepare NBCS textures");
  }
}

std::pair<ID3D11ShaderResourceView*, ID3D11ShaderResourceView*> NBCSTextureGenerator::getSRVs() const {
  return std::make_pair(m_tangentTexture->getSRV(), m_binormalTexture->getSRV());
}

std::vector<std::pair<float, float>> NBCSTextureGenerator::computeTangentScales(std::vector<rcore::Preset3D::StandardVertexType> const& vertices, std::vector<uint32_t> const& indices) {
  std::vector<DirectX::XMFLOAT3> tangentAccum(vertices.size(), { 0, 0, 0 });
  std::vector<DirectX::XMFLOAT3> binormalAccum(vertices.size(), { 0, 0, 0 });

  for (size_t i = 0; i < indices.size(); i += 3) {
    uint32_t i0 = indices[i], i1 = indices[i + 1], i2 = indices[i + 2];
    auto const& v0 = vertices[i0];
    auto const& v1 = vertices[i1];
    auto const& v2 = vertices[i2];

    DirectX::XMFLOAT3 e1 = { v1.position.x - v0.position.x, v1.position.y - v0.position.y, v1.position.z - v0.position.z };
    DirectX::XMFLOAT3 e2 = { v2.position.x - v0.position.x, v2.position.y - v0.position.y, v2.position.z - v0.position.z };

    float du1 = v1.uv.x - v0.uv.x, dv1 = v1.uv.y - v0.uv.y;
    float du2 = v2.uv.x - v0.uv.x, dv2 = v2.uv.y - v0.uv.y;

    float denom = du1 * dv2 - du2 * dv1;
    if (fabs(denom) < 1e-8f) continue; // degenerate UV triangle, skip
    float f = 1.0f / denom;

    DirectX::XMFLOAT3 tRaw = {
      f * (dv2 * e1.x - dv1 * e2.x),
      f * (dv2 * e1.y - dv1 * e2.y),
      f * (dv2 * e1.z - dv1 * e2.z)
    };
    DirectX::XMFLOAT3 bRaw = {
      f * (du1 * e2.x - du2 * e1.x),
      f * (du1 * e2.y - du2 * e1.y),
      f * (du1 * e2.z - du2 * e1.z)
    };

    for (uint32_t idx : { i0, i1, i2 }) {
      tangentAccum[idx].x += tRaw.x; tangentAccum[idx].y += tRaw.y; tangentAccum[idx].z += tRaw.z;
      binormalAccum[idx].x += bRaw.x; binormalAccum[idx].y += bRaw.y; binormalAccum[idx].z += bRaw.z;
    }
  }

  std::vector<std::pair<float, float>> scales(vertices.size());
  for (size_t i = 0; i < vertices.size(); i++) {
    float sTangent = std::sqrt(tangentAccum[i].x * tangentAccum[i].x + tangentAccum[i].y * tangentAccum[i].y + tangentAccum[i].z * tangentAccum[i].z);
    float sBinormal = std::sqrt(binormalAccum[i].x * binormalAccum[i].x + binormalAccum[i].y * binormalAccum[i].y + binormalAccum[i].z * binormalAccum[i].z);
    scales[i] = { sTangent, sBinormal };
  }

  return scales;
}

rcore::StaticIndexedVertexBuffer<NBCSTextureGenerator::ExtendedVertexType> NBCSTextureGenerator::makeExtendedSIVBuffer(std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer) {
  auto lockedSIVBuffer = sivBuffer.lock();

  auto standardVerts = lockedSIVBuffer->getVertices();
  std::vector<ExtendedVertexType> extendedVerts(standardVerts.size());
  std::vector<std::pair<float, float>> tangentScales = computeTangentScales(lockedSIVBuffer->getVertices(), lockedSIVBuffer->getIndices());

  for (int i = 0; i < standardVerts.size(); i++) {
    ExtendedVertexType& eVert = extendedVerts[i];
    static_cast<rcore::Preset3D::StandardVertexType&>(eVert) = standardVerts[i];

    auto [sTangent, sBinormal] = tangentScales.at(i);
    eVert.sTangent = sTangent;
    eVert.sBinormal = sBinormal;
  }

  return rcore::StaticIndexedVertexBuffer<NBCSTextureGenerator::ExtendedVertexType>{ extendedVerts, lockedSIVBuffer->getIndices() };
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

void NBCSTextureGenerator::bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window, rcore::StaticIndexedVertexBuffer<ExtendedVertexType> sivBuffer) {
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

  auto eSivBPtr = std::make_shared<rcore::StaticIndexedVertexBuffer<ExtendedVertexType>>(sivBuffer);
  rcore::Model model{ material(), eSivBPtr };
  model.drawIndexed();

  auto lockedWindow = window.lock();
  if (!lockedWindow) { 
    RCORE_LOG(rcore::ERR, "Encountered invalid window pointer while generating tangent and binormal textures for NBCS");
    return;
  }

  lockedWindow->bindContextStates();
  lockedWindow->activateContext();
}