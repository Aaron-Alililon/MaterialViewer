#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include <Windows.h>

#include "Core/Layer.h"
#include "Core/Rcore.h"
#include "D3D11/Buffer/MatrixBuffer.h"
#include "Core/Preset3D.h"

#include "Layer/SettingsLayer.h"
#include "Layer/TextureSettingsLayer.h"
#include "Camera/CameraController.h"
#include "Models/DisplayModel.h"
#include "Models/Skybox.h"

class ViewLayer : public rcore::Layer {

public:
  ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc);

public:
  void update(rcore::FrameState const& frame) override;
  void render(rcore::FrameState const& frame) override;
  bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;

private:
  void createMatrixBuffer();
  void createModels();
  void createCam();
  void setSkybox() const;
  void setTonemappingProperties() const;
  void createSettings();

  void makeTextureSettingsWindow(std::vector<std::string> const& paths);

public:
  int tonemapMethod = 1; // Neutral
  float exposure = 0.6f;

private:
  std::weak_ptr<rcore::Window> m_settingsWindow;
  rcore::D3DContextDesc m_ctxDesc;
  CameraController m_camController;
  std::shared_ptr<rcore::MatrixBuffer> m_matrixBuffer;
  std::unique_ptr<Skybox> m_skybox;
  std::shared_ptr<DisplayModel> m_displayModel;
};

#endif