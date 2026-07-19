#include "ViewLayer.h"

ViewLayer::ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc) : Layer(window), m_settingsWindow{ settingsWindow }, m_ctxDesc{ contextDesc } {
  createMatrixBuffer();
  createModels();
  createCam();
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

  m_skybox->render(frame, *m_matrixBuffer);
  m_modelSphere->render(frame, *m_matrixBuffer);
}

bool ViewLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  if (m_camController.onEvent(hwnd, umsg, wparam, lparam)) {
    m_camController.updatePosition();
    m_modelSphere->onCamChange(m_camController.getPosition());
  }

  switch (umsg) {
    case WM_DROPFILES: {
      HDROP hDrop = (HDROP)wparam;
      UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

      for (UINT i = 0; i < fileCount; i++) {
        wchar_t path[MAX_PATH];
        DragQueryFile(hDrop, i, path, MAX_PATH);

        std::string pathString = std::filesystem::path(path).string();
        makeTextureSettingsWindow(pathString);
      }

      DragFinish(hDrop);
      return false;
    }

    case WM_CLOSE: {
      auto lockedSettings = m_settingsWindow.lock();
      if (lockedSettings) lockedSettings->hintClose();
      break;
    }
  }

  return true;
}

void ViewLayer::createMatrixBuffer() {
  m_matrixBuffer = std::make_shared<rcore::MatrixBuffer>(0, rcore::Vertex | rcore::Pixel);
}

void ViewLayer::createModels() {
  auto inputDesc = rcore::Preset3D::makeStandardInputDescription();
  m_modelSphere = std::make_shared<ModelSphere>(m_window, inputDesc);
  m_skybox = std::make_unique<Skybox>(inputDesc);
}

void ViewLayer::createCam() {
  m_camController.updatePosition();
  m_modelSphere->onCamChange(m_camController.getPosition());
}

void ViewLayer::createSettings() const {
  auto lockedSettings = m_settingsWindow.lock();
  if (lockedSettings) {
    auto lockedSettingsLayer = lockedSettings->getLayer<SettingsLayer>().lock();
    if (lockedSettingsLayer) {

      lockedSettingsLayer->addHeading("Texture Scale");

      lockedSettingsLayer->addFloatSlider(
        "U",
        &m_modelSphere->uScale,
        0.0001f, 20,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "V",
        &m_modelSphere->vScale,
        0.0001f, 20,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addHeading("Lighting");

      lockedSettingsLayer->addFloatSlider(
        "Global Illumination Strength",
        &m_modelSphere->giStrength,
        0, 1,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addHeading("Displacement");

      lockedSettingsLayer->addDropdown(
        "Displacement Method",
        &m_modelSphere->displacementMethod,
        std::vector<std::string>{ "Vertex Offset", "Parallax Occlusion Mapping", "Normal-Based Curved Silhouettes" },
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "Displacement Strength",
        &m_modelSphere->displacement,
        0, 2,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "NBCS Step Size Factor",
        &m_modelSphere->nbcsStepSizeFactor,
        1, 10,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "POM/NBCS minimum layers",
        &m_modelSphere->minPOMLayers,
        1, 500,
        [this]() { m_modelSphere->setProperties(); }
      );

      lockedSettingsLayer->addFloatSlider(
        "POM/NBCS maximum layers",
        &m_modelSphere->maxPOMLayers,
        1, 500,
        [this]() { m_modelSphere->setProperties(); }
      );

    }
  }
}

void ViewLayer::makeTextureSettingsWindow(std::string path) {
  rcore::WindowDesc texSettingsWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - Texture Settings", 800, 800);
  auto texSettingsWindow = rcore::makeWindow(texSettingsWindDesc);

  rcore::D3DContextDesc texSettingsCtxDesc = rcore::Preset3D::makeStandardContextDescription(texSettingsWindDesc.width(), texSettingsWindDesc.height());
  rcore::makeD3D11Context(texSettingsWindow, texSettingsCtxDesc);

  texSettingsWindow.lock()->addLayer<TextureSettingsLayer>(path, m_modelSphere);
}