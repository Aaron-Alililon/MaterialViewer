#ifndef TEXTURE_SETTINGS_LAYER_H
#define TEXTURE_SETTINGS_LAYER_H

#include "Layer.h"
#include "Window.h"
#include "ModelSphere.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Setting.h"

class TextureSettingsLayer : public rcore::Layer {

public:
  TextureSettingsLayer(std::weak_ptr<rcore::Window> window, std::vector<std::string> const& texturePaths, std::weak_ptr<ModelSphere> model);

public:
  void render(rcore::FrameState const& frame) override;
  bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;
  bool isUI() const override { return true; }

  void submit();

private:
  void createTypeList();
  void createSettings();

public:
  std::vector<int> types;

private:
  ImGuiContext* m_imguiContext;
  std::vector<std::string> m_texturePaths;
  std::weak_ptr<ModelSphere> m_model;
  std::vector<std::unique_ptr<ISetting>> m_settings;
};

#endif