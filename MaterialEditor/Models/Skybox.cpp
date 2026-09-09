#include "Models/Skybox.h"

Skybox::Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  createSIVBuffer();
  createSampler();
  createMaterial(inputDesc);
  createModel();
}

void Skybox::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_box->drawIndexed(matrixBuffer);
}

void Skybox::setType(SkyboxType type) {
  m_type = getSkybox(type, 8, 2);

  ID3D11ShaderResourceView* srv[] = { m_type.textureAlbedo->getTextureView() };
  m_material->setTextures(srv, 0);

  m_type.directionalLightBuffer->uploadBuffer();
}

void Skybox::createSIVBuffer() {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::ObjLoader>("models/skybox.obj");
}

void Skybox::createSampler() {
  m_sampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());
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

Skybox::SkyboxData Skybox::getSkybox(SkyboxType type, int srvSlot, int numLightsBufferSlot, uint8_t shaderStages) const {
  Skybox::SkyboxData data;
  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();
  data.directionalLightBuffer = std::make_shared<rcore::LightBuffer>(srvSlot, numLightsBufferSlot, shaderStages);

  switch (type) {
    case sky:
    {
      data.textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/sky.png", texDesc, srvDesc);

      rcore::LightBufferType directionals[] = {
        {{ 0, 0, 0, 0 }, { 1, 0.5f, 0, 0 }, { 1, 0.976f, 0.925f, 1 }},
        {{ 0, 0, 0, 0 }, { 1, -0.5f, 0, 0 }, { 0.9f, 0.876f, 0.825f, 1 }}
      };
      data.directionalLightBuffer->setData(directionals);

      break;
    }
    case forest: {
      data.textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/field.png", texDesc, srvDesc);

      rcore::LightBufferType directionals[] = {
        {{ 0, 0, 0, 0 }, { 0.8f, 1, 1, 0 }, { 1, 1, 1, 1 }},
      };
      data.directionalLightBuffer->setData(directionals);

      break;
    }
  }

  return data;
}