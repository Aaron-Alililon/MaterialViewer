#ifndef SETTINGS_LAYER_H
#define SETTINGS_LAYER_H

#include <functional>

#include "Core/Layer.h"
#include "D3D11/D3DContextDesc.h"
#include "D3D11/D3D11Device.h"
#include "Window/Window.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"

#include "Layer/Setting.h"

class SettingsLayer : public rcore::Layer {
public:
  void addHeading(std::string label);
  void addFloatSlider(std::string label, float* value, float min, float max, std::function<void()> onChange = nullptr);
  void addCheckbox(std::string label, bool* value, std::function<void()> onChange = nullptr);
  void addDropdown(std::string label, int* selectedIndex, std::vector<std::string> options, std::function<void()> onChange = nullptr);

public:
  SettingsLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc);

public:
  void render(rcore::FrameState const& frame) override;
  bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) override;

  bool isUI() const override { return true; }

private:
  ImGuiContext* m_imguiContext;
  std::vector<std::unique_ptr<ISetting>> m_settings;
};

#endif