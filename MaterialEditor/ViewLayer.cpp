#include "ViewLayer.h"

ViewLayer::ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc) : Layer(window), m_settingsWindow{ settingsWindow }, m_ctxDesc { contextDesc } {
  auto inputDesc = rcore::Preset3D::makeStandardInputDescription();

  m_matrixBuffer = std::make_shared<rcore::MatrixBuffer>(0);
  m_directionalLightBuffer = std::make_shared<rcore::LightBuffer>(6, 2);
  m_modelSphere = std::make_unique<ModelSphere>(inputDesc);
  m_skybox = std::make_unique<Skybox>(inputDesc);

  rcore::LightBufferType directionals[] = {
    {{ 0, 0, 0, 0 }, { 1, 0.5f, 0, 0 }, { 1, 0.976f, 0.925f, 1 }},
    {{ 0, 0, 0, 0 }, { 1, -0.5f, 0, 0 }, { 0.9f, 0.876f, 0.825f, 1 }}
  };
  m_directionalLightBuffer->setData(directionals);
  m_directionalLightBuffer->uploadBuffer();

  m_camController.updatePosition();
  m_modelSphere->onCamChange(m_camController.getPosition());

  createSettings();
}

void ViewLayer::update(rcore::FrameState const& frame) {
  m_camController.setMatrices(m_matrixBuffer, (float)frame.width / frame.height);
}

void ViewLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getSceneRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  m_modelSphere->render(frame, *m_matrixBuffer);
  m_skybox->render(frame, *m_matrixBuffer);
}

bool ViewLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  if (m_camController.onEvent(hwnd, umsg, wparam, lparam)) {
    m_camController.updatePosition();
    m_modelSphere->onCamChange(m_camController.getPosition());
  }

  switch (umsg) {
    case WM_CLOSE: {
      auto lockedSettings = m_settingsWindow.lock();
      if (lockedSettings) lockedSettings->hintClose();
      break;
    }
  }

  return true;
}

void ViewLayer::createSettings() const {
  auto lockedSettings = m_settingsWindow.lock();
  if (lockedSettings) {
    auto lockedSettingsLayer = lockedSettings->getLayer<SettingsLayer>().lock();
    if (lockedSettingsLayer) {

      lockedSettingsLayer->addFloatSlider(
        "Texture u scale",
        &m_modelSphere->uScale,
        0.0001f, 20,
        [this]() { m_modelSphere->onValueChange(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "Texture v scale",
        &m_modelSphere->vScale,
        0.0001f, 20,
        [this]() { m_modelSphere->onValueChange(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "Displacement strength",
        &m_modelSphere->displacement,
        0, 1,
        [this]() { m_modelSphere->onValueChange(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "Global Illumination Strength",
        &m_modelSphere->giStrength,
        0, 1,
        [this]() { m_modelSphere->onValueChange(); }
      );

    }
  }
}