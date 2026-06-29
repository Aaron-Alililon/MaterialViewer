#include "ModelSphere.h"

ModelSphere::ModelSphere(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc) {
  rcore::Shader shader = { L"VertexShader.hlsl", L"PixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, 1, rcore::Pixel);
  m_material->setProperties({
    { 0.4f, 0.5f, 1.0f, 1.0f },
    { 1.0f, 1.0f, -1.0f, 0.0f },
    0.1f
  });

  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer("models/monkey.obj");

  m_model = std::make_unique<rcore::Model>(m_material, m_SIVBuffer);
}

void ModelSphere::render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer) {
  m_model->setRotation(0, frame.frameCount * 0.01f, 0);
  m_model->drawIndexed(matrixBuffer);
}