#include "ViewLayer.h"

ViewLayer::ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc) : Layer(window), m_settingsWindow{ settingsWindow }, m_ctxDesc { contextDesc } {
  auto inputDesc = rcore::Preset3D::makeStandardInputDescription();

  m_matrixBuffer = std::make_shared<rcore::MatrixBuffer>(0);
  m_modelSphere = std::make_unique<ModelSphere>(inputDesc);
  m_skybox = std::make_unique<Skybox>(inputDesc);

  auto lockedSettings = m_settingsWindow.lock();
  if (lockedSettings) {
    auto lockedSettingsLayer = lockedSettings->getLayer<SettingsLayer>().lock();
    if (lockedSettingsLayer) {

      lockedSettingsLayer->addFloatSlider(
        "Displacement strength",
        &m_modelSphere->displacement,
        0, 1,
        [this](float newValue) { m_modelSphere->onDisplacementChange(newValue); }
      );

    }
  }

  
}

void ViewLayer::update(rcore::FrameState const& frame) {
  m_camController.setMatrices(m_matrixBuffer, (float)frame.width / frame.height);
}

void ViewLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  m_modelSphere->render(frame, *m_matrixBuffer);
  m_skybox->render(frame, *m_matrixBuffer);
}

bool ViewLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  m_camController.onEvent(hwnd, umsg, wparam, lparam);

  switch (umsg) {
    case WM_CLOSE: {
      auto lockedSettings = m_settingsWindow.lock();
      if (lockedSettings) lockedSettings->hintClose();
      break;
    }
  }

  return true;
}