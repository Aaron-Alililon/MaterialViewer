#include "SettingsLayer.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  static float value = 0.5f;
  ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
  ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoMove);
  ImGui::SliderFloat("Slider", &value, 0.0f, 1.0f);
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