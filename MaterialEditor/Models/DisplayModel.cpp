#include "Models/DisplayModel.h"

DisplayModel::DisplayModel(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  createTextures();
  createMaterial(inputDesc);
  createModels(window);
  createCam();
  setModel();
  setProperties();
}

void DisplayModel::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_models[selectedModel].model->drawIndexed(matrixBuffer);
}

void DisplayModel::setModel() {
  bindTextures();
}

void DisplayModel::setProperties() {
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

void DisplayModel::onCamChange(DirectX::XMFLOAT3 pos) const {
  m_camBuffer->setData({ { pos.x, pos.y, pos.z, 1 } });
  m_camBuffer->uploadBuffer();
}

void DisplayModel::updateTexture(std::string const& path, TextureType type) {
  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  m_textures[static_cast<size_t>(type)] = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, path, texDesc, srvDesc);
  
  bindTextures();
}

void DisplayModel::createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"shaders/PBRVertexShader.hlsl", L"shaders/PBRPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel | rcore::Vertex);

  ID3D11SamplerState* sState[] = { m_pointSampler->getSamplerState(), m_linearSampler->getSamplerState() };
  m_material->setSamplers(sState, 0);
}

void DisplayModel::createTextures() {
  std::string textureFamily = "default";
  static constexpr std::array<const char*, 6> textureNames = { "albedo", "normal", "displacement", "roughness", "metallic", "ambientOcclusion" };

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  for (size_t i = 0; i < textureNames.size(); i++) {
    std::string texPath = "textures/" + textureFamily + "/" + textureNames[i] + ".png";
    m_textures[i] = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, texPath, texDesc, srvDesc);
  }

  m_pointSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardPointSamplerDescription());
  m_linearSampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());
}

void DisplayModel::createSIVBuffer(ModelData& data, std::string const& path) {
  data.SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::GLTFLoader>(path, true);
}

void DisplayModel::createNbcsTextures(ModelData& data, std::weak_ptr<rcore::Window> window) {
  auto lockedWindow = window.lock();
  if (!lockedWindow) {
    RCORE_LOG(rcore::ERR, "Tried creating NBCS textures with invalid window pointer");
    return;
  }

  NBCSTextureGenerator nbcsTexGen{ lockedWindow, data.SIVBuffer };
  data.nbcsTextures = nbcsTexGen.getTextures();
}

void DisplayModel::createModels(std::weak_ptr<rcore::Window> window) {
  static constexpr std::array<const char*, 2> modelPaths = { "models/sphere.glb", "models/plane.glb" };

  for (size_t i = 0; i < modelPaths.size(); i++) {
    ModelData data{};

    createSIVBuffer(data, modelPaths[i]);
    createNbcsTextures(data, window);
    data.model = std::make_unique<rcore::Model>(m_material, data.SIVBuffer);

    m_models.push_back(std::move(data));
  }
}

void DisplayModel::createCam() {
  m_camBuffer = std::make_unique<rcore::CBuffer<CameraBufferData>>(3, rcore::Pixel);
  m_camBuffer->setData({ });
}

void DisplayModel::bindTextures() {
  ID3D11ShaderResourceView* srv[] = {
    m_textures[0]->getTextureView(), m_textures[1]->getTextureView(), m_textures[2]->getTextureView(),
    m_textures[3]->getTextureView(), m_textures[4]->getTextureView(), m_textures[5]->getTextureView(),
    m_models[selectedModel].nbcsTextures.first->getSRV(), m_models[selectedModel].nbcsTextures.second->getSRV()
  };

  m_material->setTextures(srv, 0);
}