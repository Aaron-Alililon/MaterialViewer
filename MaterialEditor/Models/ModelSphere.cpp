#include "Models/ModelSphere.h"

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

  m_textures[static_cast<size_t>(type)] = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, path, texDesc, srvDesc);
  
  bindTextures();
}

void ModelSphere::createSIVBuffer() {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::GLTFLoader>("models/sphere.glb", true);
}

void ModelSphere::createTextures(std::weak_ptr<rcore::Window> window) {
  std::string textureFamily = "default";
  static constexpr std::array<const char*, 6> textureNames = { "albedo", "normal", "displacement", "roughness", "metallic", "ambientOcclusion" };

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  for (size_t i = 0; i < textureNames.size(); i++) {
    std::string texPath = "textures/" + textureFamily + "/" + textureNames[i] + ".png";
    m_textures[i] = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, texPath, texDesc, srvDesc);
  }

  m_pointSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardPointSamplerDescription());
  m_linearSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());

  auto lockedWindow = window.lock();

  NBCSTextureGenerator nbcsTexGen{ lockedWindow, m_SIVBuffer };
  m_nbcsTextures = nbcsTexGen.getTextures();
}

void ModelSphere::createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"shaders/PBRVertexShader.hlsl", L"shaders/PBRPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel | rcore::Vertex);

  ID3D11SamplerState* sState[] = { m_pointSampler->getSamplerState(), m_linearSampler->getSamplerState() };
  m_material->setSamplers(sState, 0);

  bindTextures();
}

void ModelSphere::createCam() {
  m_camBuffer = std::make_unique<rcore::CBuffer<CameraBufferData>>(3, rcore::Pixel);
  m_camBuffer->setData({ });
}

void ModelSphere::createModel() {
  m_sphere = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
}

void ModelSphere::bindTextures() {
  ID3D11ShaderResourceView* srv[] = {
    m_textures[0]->getTextureView(), m_textures[1]->getTextureView(), m_textures[2]->getTextureView(),
    m_textures[3]->getTextureView(), m_textures[4]->getTextureView(), m_textures[5]->getTextureView(),
    m_nbcsTextures.first->getSRV(), m_nbcsTextures.second->getSRV()
  };

  m_material->setTextures(srv, 0);
}