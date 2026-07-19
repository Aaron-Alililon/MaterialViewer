#include "ModelSphere.h"

ModelSphere::ModelSphere(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  createSIVBuffer();
  createTextures(window);
  createMaterial(inputDesc);
  createCam();
  createModel();
  setProperties();
}

void ModelSphere::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_sphere->drawIndexed(matrixBuffer);
}

void ModelSphere::setProperties() {
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

void ModelSphere::updateTexture(std::string const& path, TextureType type) {
  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  auto tex = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, path, texDesc, srvDesc);

  switch (type) {
    case TextureType::albedo:
      m_textureAlbedo = tex;
      break;
    case TextureType::normal:
      m_textureNormal = tex;
      break;
    case TextureType::displacement:
      m_textureDisplacement = tex;
      break;
    case TextureType::roughness:
      m_textureRoughness = tex;
      break;
    case TextureType::metallic:
      m_textureMetallic = tex;
      break;
    case TextureType::ambientOcclusion:
      m_textureAmbientOcclusion = tex;
      break;
  }
  
  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView(), m_textureNormal->getTextureView(), m_textureDisplacement->getTextureView(), m_textureRoughness->getTextureView(), m_textureMetallic->getTextureView(), m_textureAmbientOcclusion->getTextureView(), m_nbcsTextures.first->getSRV(), m_nbcsTextures.second->getSRV() };
  m_material->setTextures(srv, 0);
}

void ModelSphere::createSIVBuffer() {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::GLTFLoader>("models/sphere.glb", true);
}

void ModelSphere::createTextures(std::weak_ptr<rcore::Window> window) {
  std::string textureFamily = "default";

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  m_textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/albedo.png", texDesc, srvDesc);
  m_textureNormal = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/normal.png", texDesc, srvDesc);
  m_textureDisplacement = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/displacement.png", texDesc, srvDesc);
  m_textureRoughness = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/roughness.png", texDesc, srvDesc);
  m_textureMetallic = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/metallic.png", texDesc, srvDesc);
  m_textureAmbientOcclusion = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/" + textureFamily + "/ambientOcclusion.png", texDesc, srvDesc);

  m_pointSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardPointSamplerDescription());
  m_linearSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());

  auto lockedWindow = window.lock();

  NBCSTextureGenerator nbcsTexGen{ lockedWindow, m_SIVBuffer };
  m_nbcsTextures = nbcsTexGen.getTextures();
}

void ModelSphere::createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"PBRVertexShader.hlsl", L"PBRPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel | rcore::Vertex);

  ID3D11SamplerState* sState[] = { m_pointSampler->getSamplerState(), m_linearSampler->getSamplerState() };
  m_material->setSamplers(sState, 0);

  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView(), m_textureNormal->getTextureView(), m_textureDisplacement->getTextureView(), m_textureRoughness->getTextureView(), m_textureMetallic->getTextureView(), m_textureAmbientOcclusion->getTextureView(), m_nbcsTextures.first->getSRV(), m_nbcsTextures.second->getSRV() };
  m_material->setTextures(srv, 0);
}

void ModelSphere::createCam() {
  m_camBuffer = std::make_unique<rcore::CBuffer<CameraBufferData>>(3, rcore::Pixel);
  m_camBuffer->setData({ });
}

void ModelSphere::createModel() {
  m_sphere = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
}