#include "TextureSettingsLayer.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

TextureSettingsLayer::TextureSettingsLayer(std::weak_ptr<rcore::Window> window, std::string texturePath, std::weak_ptr<ModelSphere> model) : Layer{ window }, m_texturePath{ texturePath }, m_model{ model } {
  m_imguiContext = ImGui::CreateContext();
  ImGui::SetCurrentContext(m_imguiContext);
  ImGui_ImplWin32_Init(window.lock()->getHandle());
  ImGui_ImplDX11_Init(rcore::D3D11Device::get().raw(), rcore::D3D11Device::get().rawContext());

  createSettings();
}

void TextureSettingsLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  ImGui::SetCurrentContext(m_imguiContext);
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

  ImGui::Begin("Texture Settings", nullptr,
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

bool TextureSettingsLayer::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  ImGui::SetCurrentContext(m_imguiContext);

  if (ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam)) {
    return false;
  }

  return true;
}

void TextureSettingsLayer::submit() {
  auto lockedModel = m_model.lock();
  if (!lockedModel) return;

  lockedModel->updateTexture(m_texturePath, static_cast<TextureType>(type));

  m_window.lock()->hintClose();
}

void TextureSettingsLayer::createSettings() {
  m_settings.push_back(std::make_unique<TextSetting>(m_texturePath));

  m_settings.push_back(std::make_unique<DropdownSetting>(
    "Texture Type",
    &type,
    std::vector<std::string>{ 
      "Albedo",
      "Normal",
      "Displacement",
      "Roughness",
      "Metallic",
      "Ambient Occlusion"
    }
  ));

  m_settings.push_back(std::make_unique<ButtonSetting>(
    "Submit",
    [this]() { submit(); }
  ));
}