#include "ModelSphere.h"

ModelSphere::ModelSphere(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::GLTFLoader>("models/sphere.glb", true);

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();
  std::string textureFamily = "paving";
  m_textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/albedo.png", texDesc, srvDesc);
  m_textureNormal = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/normal.png", texDesc, srvDesc);
  m_textureDisplacement = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/displacement.png", texDesc, srvDesc);
  m_textureRoughness = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/roughness.png", texDesc, srvDesc);
  m_textureMetallic = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/metallic.png", texDesc, srvDesc);
  m_textureAmbientOcclusion = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/ambientOcclusion.png", texDesc, srvDesc);

  m_pointSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardPointSamplerDescription());
  m_linearSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());

  rcore::Shader shader = { L"PBRVertexShader.hlsl", L"PBRPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel | rcore::Vertex);

  ID3D11SamplerState* sState[] = { m_pointSampler->getSamplerState(), m_linearSampler->getSamplerState() };
  m_material->setSamplers(sState, 0);

  m_camBuffer = std::make_unique<rcore::CBuffer<CameraBufferData>>(3, rcore::Pixel);
  m_camBuffer->setData({ });

  m_sphere = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);

  onValueChange();
}

void ModelSphere::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_sphere->drawIndexed(matrixBuffer);
}

void ModelSphere::onValueChange() {
  m_properties = {
    { uScale, vScale },
    giStrength,
    displacement,
    displacementMethod,
    nbcsStepSizeFactor,
    { minPOMLayers, maxPOMLayers }
  };

  m_material->uploadProperties(m_properties, 1);
}

void ModelSphere::onCamChange(DirectX::XMFLOAT3 pos) const {
  m_camBuffer->setData({ { pos.x, pos.y, pos.z, 1 } });
  m_camBuffer->uploadBuffer();
}

void ModelSphere::onNBCSBakeFinish(ID3D11ShaderResourceView* normalMapSRV, ID3D11ShaderResourceView* tangentMapSRV) const {
  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView(), m_textureNormal->getTextureView(), m_textureDisplacement->getTextureView(), m_textureRoughness->getTextureView(), m_textureMetallic->getTextureView(), m_textureAmbientOcclusion->getTextureView(), normalMapSRV, tangentMapSRV };
  m_material->setTextures(srv, 0);
}

std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> ModelSphere::getSIVBuffer() const {
  return m_SIVBuffer;
}