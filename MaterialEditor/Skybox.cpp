#include "Skybox.h"

Skybox::Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer<rcore::ObjLoader>("models/skybox.obj");

  auto [texDesc, srvDesc] = rcore::Preset3D::makeStandardTextureDescriptionPair();
  m_textureAlbedo = std::make_shared<rcore::Texture>(rcore::LoaderTag<rcore::PNGLoader>{}, "textures/skybox/cubemap.png", texDesc, srvDesc);

  m_sampler = std::make_shared<rcore::Sampler>(rcore::Preset3D::makeStandardLinearSamplerDescription());

  rcore::Shader shader = { L"SkyboxVertexShader.hlsl", L"SkyboxPixelShader.hlsl", inputDesc };

  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, 3, rcore::Pixel);

  m_box = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
  m_box->setScale(1000, 1000, 1000);
}

void Skybox::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  ID3D11ShaderResourceView* srv[] = { m_textureAlbedo->getTextureView() };
  m_material->uploadTextures(srv, 0);
  m_box->drawIndexed(matrixBuffer);
}