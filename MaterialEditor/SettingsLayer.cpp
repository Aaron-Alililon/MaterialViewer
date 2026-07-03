#include "SettingsLayer.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SettingsLayer::addFloatSlider(std::string label, float* value, float min, float max, std::function<void()> onChange) {
  m_settings.push_back(std::make_unique<FloatSliderSetting>(std::move(label), value, min, max, onChange));
}

SettingsLayer::SettingsLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc) : Layer{window} {
  ImGui::CreateContext();
  ImGui_ImplWin32_Init(window.lock()->getHandle());
  ImGui_ImplDX11_Init(rcore::D3D11Device::get().raw(), rcore::D3D11Device::get().rawContext());
}

void SettingsLayer::update(rcore::FrameState const& frame) {
  
}

void SettingsLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getUIRenderTargetView(), bgCol);

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

  ImGui::Begin("Settings", nullptr,
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoTitleBar
  );

  for (auto& setting : m_settings) {
    setting->draw();
  }

  ImGui::End();

  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool SettingsLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  if (ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam)) {
    return false;
  }

  return true;
}