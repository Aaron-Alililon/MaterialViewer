#include "EditorLayer.h"

EditorLayer::EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc) : Layer(window), m_ctxDesc{ contextDesc } {
  auto inputDesc = rcore::Preset3D::makeStandardInputDescription();

  m_cam = { 0, 0, -10 };

  rcore::Shader shader = { L"VertexShader.hlsl", L"PixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, 1, rcore::Pixel);
  m_material->setProperties({
    { 0.4f, 0.5f, 1.0f, 1.0f },
    { 1.0f, 1.0f, -1.0f, 0.0f },
    0.1f
  });

  rcore::Transform transform{
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 1, 1, 1 }
  };

  m_SIVBuffer = rcore::Preset3D::makeStandardSIVBuffer("models/monkey.obj");
  
  m_model = std::make_unique<rcore::Model>(transform, m_material, m_SIVBuffer);

  m_matrixBuffer = std::make_shared<rcore::MatrixBuffer>(0);
}

void EditorLayer::update(rcore::FrameState const& frame) {
  m_matrixBuffer->setProjectionMatrix(m_cam.getPerspectiveMatrix((float)frame.width / frame.height));
  m_matrixBuffer->setViewMatrix(m_cam.getViewMatrix());
}

void EditorLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  m_model->setRotation(0, frame.frameCount * 0.01f, 0);
  m_model->drawIndexed(*m_matrixBuffer);
}