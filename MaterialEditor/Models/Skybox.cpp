#include "Models/Skybox.h"

Skybox::Skybox(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  bakeIBL(window);
  createSIVBuffer();
  createSampler();
  createMaterial(inputDesc);
  createModel();
}

void Skybox::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_box->drawIndexed(matrixBuffer);
}

Skybox::SkyboxData Skybox::setType() {
  ID3D11ShaderResourceView* srv[] = { m_skyboxes.at(selectedSkybox).envCube->getSRV() };
  m_material->setTextures(srv, 0);

  return m_skyboxes.at(selectedSkybox);
}

void Skybox::bakeIBL(std::weak_ptr<rcore::Window> window) {
  static const std::string skyboxPaths[] = { "sky.hdr", "forest.hdr", "livingRoom.hdr", "studio.hdr" };

  for (auto const& path : skyboxPaths) {
    SkyboxData data{ };

    EquirectToCubeBaker etcb{ window, "skyboxes/" + path };
    data.envCube = etcb.getEnvironmentCube();

    IrradianceBaker ib{ window, data.envCube, etcb.getCacheKey() };
    data.irradianceCube = ib.getIrradianceCube();

    SpecularBaker sb{ window, data.envCube, etcb.getCacheKey() };
    data.specularCube = sb.getPrefilteredCube();

    BRDFLUTBaker lutb{ window };
    data.brdfLut = lutb.getLUT();

    m_skyboxes.push_back(data);
  }

  BakePass::pruneCache();
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