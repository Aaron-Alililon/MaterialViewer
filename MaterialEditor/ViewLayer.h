#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "MatrixBuffer.h"
#include "Preset3D.h"

#include "SettingsLayer.h"
#include "CameraController.h"
#include "ModelSphere.h"
#include "Skybox.h"

class ViewLayer : public rcore::Layer {

public:
  ViewLayer(std::weak_ptr<rcore::Window> window, std::weak_ptr<rcore::Window> settingsWindow, rcore::D3DContextDesc contextDesc);

public:
  void update(rcore::FrameState const& frame) override;
  void render(rcore::FrameState const& frame) override;
  bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;

private:
  std::weak_ptr<rcore::Window> m_settingsWindow;
  rcore::D3DContextDesc m_ctxDesc;
  CameraController m_camController;
  std::shared_ptr<rcore::MatrixBuffer> m_matrixBuffer;
  std::unique_ptr<ModelSphere> m_modelSphere;
  std::unique_ptr<Skybox> m_skybox;
};

#endif