#include "Models/Skybox.h"

Skybox::Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  createSIVBuffer();
  createSampler();
  createTextures();
  createMaterial(inputDesc);
  createModel();

  setType();
}

void Skybox::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_box->drawIndexed(matrixBuffer);
}

void Skybox::setType() {
  ID3D11ShaderResourceView* srv[] = { m_textures.at(selectedSkybox).getTextureView() };
  m_material->setTextures(srv, 0);
}

void Skybox::createSIVBuffer() {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::ObjLoader>("models/skybox.obj");
}

void Skybox::createSampler() {
  m_sampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());
}

void Skybox::createTextures() {
  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();

  m_textures.push_back({ rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/sky.png", texDesc, srvDesc });
  m_textures.push_back({ rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/field.png", texDesc, srvDesc });
}

void Skybox::createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"shaders/SkyboxVertexShader.hlsl", L"shaders/SkyboxPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel);

  ID3D11SamplerState* sState[] = { m_sampler->getSamplerState() };
  m_material->setSamplers(sState, 0);
}

void Skybox::createModel() {
  m_box = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
  m_box->setScale(1000, 1000, 1000);
}