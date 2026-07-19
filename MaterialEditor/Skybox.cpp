#include "Skybox.h"

Skybox::Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  createSIVBuffer();
  createTexture();
  createMaterial(inputDesc);
  createModel();
  createLights();
}

void Skybox::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_box->drawIndexed(matrixBuffer);
}

void Skybox::createSIVBuffer() {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::ObjLoader>("models/skybox.obj");
}

void Skybox::createTexture() {
  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();
  m_textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/cubemap.png", texDesc, srvDesc);

  m_sampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());
}

void Skybox::createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"SkyboxVertexShader.hlsl", L"SkyboxPixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, rcore::Pixel);

  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView() };
  m_material->setTextures(srv, 0);
}

void Skybox::createModel() {
  m_box = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
  m_box->setScale(1000, 1000, 1000);
}

void Skybox::createLights() {
  m_directionalLightBuffer = std::make_shared<rcore::LightBuffer>(8, 2);

  rcore::LightBufferType directionals[] = {
    {{ 0, 0, 0, 0 }, { 1, 0.5f, 0, 0 }, { 1, 0.976f, 0.925f, 1 }},
    {{ 0, 0, 0, 0 }, { 1, -0.5f, 0, 0 }, { 0.9f, 0.876f, 0.825f, 1 }}
  };

  m_directionalLightBuffer->setData(directionals);
  m_directionalLightBuffer->uploadBuffer();
}