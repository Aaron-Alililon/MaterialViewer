#include "EditorLayer.h"

EditorLayer::EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc) : Layer(window), m_ctxDesc{ contextDesc } {
  auto inputDesc = rcore::Preset3D::makeStandardInputDescription();

  m_cam = { 0, 0, -10 };
  m_matrixBuffer = std::make_shared<rcore::MatrixBuffer>(0);
  m_modelSphere = std::make_unique<ModelSphere>(inputDesc);
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

  m_modelSphere->render(frame, *m_matrixBuffer);
}