#include "Layer/ViewLayer.h"

ViewLayer::ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc) : Layer(window), m_settingsWindow{ settingsWindow }, m_ctxDesc{ contextDesc } {
  createMatrixBuffer();
  createModels();
  createCam();
  createSettings();
  setTonemappingProperties();
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
  m_displayModel->render(frame, *m_matrixBuffer);
}

bool ViewLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  if (m_camController.onEvent(hwnd, umsg, wparam, lparam)) {
    m_camController.updatePosition();
    m_displayModel->onCamChange(m_camController.getPosition());
  }

  switch (umsg) {
    case WM_DROPFILES: {
      HDROP hDrop = (HDROP)wparam;
      UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

      std::vector<std::string> files{ fileCount };

      for (UINT i = 0; i < fileCount; i++) {
        wchar_t path[MAX_PATH];
        DragQueryFile(hDrop, i, path, MAX_PATH);

        std::string pathString = std::filesystem::path(path).string();
        files[i] = pathString;
      }

      makeTextureSettingsWindow(files);

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
  m_skybox = std::make_unique<Skybox>(m_window, inputDesc);
  m_displayModel = std::make_shared<DisplayModel>(m_window, inputDesc);

  BakePass::pruneCache();

  Skybox::SkyboxData data = m_skybox->setType();
  m_displayModel->updateSkyboxData(data);
}

void ViewLayer::createCam() {
  m_camController.updatePosition();
  m_displayModel->onCamChange(m_camController.getPosition());
}

void ViewLayer::setSkybox() const {
  Skybox::SkyboxData data = m_skybox->setType();
  m_displayModel->updateSkyboxData(data);
}

void ViewLayer::setTonemappingProperties() const {
  m_displayModel->tonemapMethod = tonemapMethod;
  m_displayModel->exposure = exposure;
  m_displayModel->setProperties();

  m_skybox->tonemapMethod = tonemapMethod;
  m_skybox->exposure = exposure;
  m_skybox->setProperties();
}

void ViewLayer::createSettings() {
  auto lockedSettings = m_settingsWindow.lock();
  auto lockedSettingsLayer = lockedSettings->getLayer<SettingsLayer>().lock();
  if (!lockedSettings || !lockedSettingsLayer) return;

  lockedSettingsLayer->addHeading("Scene");

  lockedSettingsLayer->addDropdown(
    "Model",
    &m_displayModel->selectedModel,
    std::vector<std::string>{ "Sphere", "Plane" },
    [this]() { m_displayModel->setModel(); }
  );

  lockedSettingsLayer->addDropdown(
    "Skybox",
    &m_skybox->selectedSkybox,
    std::vector<std::string>{ "Sky", "Forest", "Living Room", "Studio" },
    [this]() { setSkybox(); }
  );

  lockedSettingsLayer->addHeading("Texture Scale");

  lockedSettingsLayer->addFloatSlider(
    "U",
    &m_displayModel->uScale,
    0.0001f, 20,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "V",
    &m_displayModel->vScale,
    0.0001f, 20,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addHeading("Lighting");

  lockedSettingsLayer->addFloatSlider(
    "Global Illumination Strength",
    &m_displayModel->giStrength,
    0, 2,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addDropdown(
    "Tonemapping Method",
    &tonemapMethod,
    std::vector<std::string>{ "None", "Neutral", "ACES" },
    [this]() { setTonemappingProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "Exposure",
    &exposure,
    0, 3,
    [this]() { setTonemappingProperties(); }
  );

  lockedSettingsLayer->addHeading("Displacement");

  lockedSettingsLayer->addDropdown(
    "Displacement Method",
    &m_displayModel->displacementMethod,
    std::vector<std::string>{ "Vertex Offset", "Parallax Occlusion Mapping", "Normal-Based Curved Silhouettes" },
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "Displacement Strength",
    &m_displayModel->displacement,
    0, 2,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "NBCS Step Size Factor",
    &m_displayModel->nbcsStepSizeFactor,
    1, 10,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "POM/NBCS minimum layers",
    &m_displayModel->minPOMLayers,
    1, 500,
    [this]() { m_displayModel->setProperties(); }
  );

  lockedSettingsLayer->addFloatSlider(
    "POM/NBCS maximum layers",
    &m_displayModel->maxPOMLayers,
    1, 500,
    [this]() { m_displayModel->setProperties(); }
  );
}

void ViewLayer::makeTextureSettingsWindow(std::vector<std::string> const& paths) {
  rcore::WindowDesc texSettingsWindDesc = rcore::Preset3D::makeStandardWindowDescription(L"Material Editor - Texture Settings", 800, 800);
  auto texSettingsWindow = rcore::makeWindow(texSettingsWindDesc);

  rcore::D3DContextDesc texSettingsCtxDesc = rcore::Preset3D::makeStandardContextDescription(texSettingsWindDesc.width(), texSettingsWindDesc.height());
  rcore::makeD3D11Context(texSettingsWindow, texSettingsCtxDesc);

  texSettingsWindow.lock()->addLayer<TextureSettingsLayer>(paths, m_displayModel);
}