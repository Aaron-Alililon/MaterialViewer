#include "ModelSphere.h"

ModelSphere::ModelSphere(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::GLTFLoader>("models/sphere_extreme.glb");

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();
  m_textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/rocks/albedo.png", texDesc, srvDesc);
  m_textureNormal = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/rocks/normal.png", texDesc, srvDesc);
  m_textureDisplacement = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/rocks/displacement.png", texDesc, srvDesc);
  m_textureRoughness = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/rocks/roughness.png", texDesc, srvDesc);

  m_sampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());

  rcore::Shader shader = { L"PBRVertexShader.hlsl", L"PBRPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, 1, rcore::Pixel | rcore::Vertex);

  m_sphere = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);

  onValueChange();
}

void ModelSphere::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView(), m_textureNormal->getTextureView(), m_textureDisplacement->getTextureView(), m_textureRoughness->getTextureView() };
  m_material->uploadTextures(srv, 0);

  ID3D11SamplerState* sState[] = { m_sampler->getSamplerState() };
  m_material->uploadSamplers(sState, 0);

  m_sphere->drawIndexed(matrixBuffer);
}

void ModelSphere::onValueChange() {
  m_properties = {
    { 1.0f, 1.0f, 0.0f, 0.0f },
    { uScale, vScale },
    giStrength,
    displacement
  };

  m_material->setProperties(m_properties);
}