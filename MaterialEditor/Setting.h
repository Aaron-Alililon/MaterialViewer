#ifndef SETTING_H
#define SETTING_H

#include <string>
#include <sstream>

#include "imgui.h"

inline void drawLabelAbove(std::string const& label) {
  ImGui::TextUnformatted(label.c_str());
}

inline std::string bar(size_t length) {
  std::stringstream barString;
  for (size_t i = 0; i < length; i++) barString << "=";
  return barString.str();
}

class ISetting {
public:
  virtual ~ISetting() = default;
  virtual void draw() = 0;
};

class HeaderSetting : public ISetting {
public:
  HeaderSetting(std::string label) : m_label{ label } {}

  void draw() override {
    ImGui::Spacing();

    std::string formattedHeading;
    formattedHeading += "\n ===" + bar(m_label.length()) + "=== ";
    formattedHeading += "\n || " + m_label + " || ";
    formattedHeading += "\n ===" + bar(m_label.length()) + "=== ";

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 1, pos.y), ImGui::GetColorU32(ImGuiCol_Text), formattedHeading.c_str());
    ImGui::TextUnformatted(formattedHeading.c_str());

    ImGui::Spacing();
  }

private:
  std::string m_label;
};

class FloatSliderSetting : public ISetting {
public:
  FloatSliderSetting(std::string label, float* value, float min, float max, std::function<void()> onChange = nullptr)
    : m_label(std::move(label)), m_value(value), m_min(min), m_max(max), m_onChange{ std::move(onChange) } {}

  void draw() override {
    ImGui::Spacing();
    ImGui::Indent(4.0f);

    drawLabelAbove(m_label);
    std::string hiddenId = "##" + m_label;

    ImGui::SetNextItemWidth(-1);

    if (ImGui::SliderFloat(hiddenId.c_str(), m_value, m_min, m_max)) {
      if (m_onChange) m_onChange();
    }

    ImGui::Unindent(4.0f);
    ImGui::Spacing();
  }

private:
  std::string m_label;
  float* m_value;
  float m_min, m_max;
  std::function<void()> m_onChange;
};

class CheckboxSetting : public ISetting {
public:
  CheckboxSetting(std::string label, bool* value, std::function<void()> onChange)
    : m_label(std::move(label)), m_value(value) {}

  void draw() override {
    ImGui::Spacing();
    ImGui::Indent(4.0f);

    drawLabelAbove(m_label);
    std::string hiddenId = "##" + m_label;
    ImGui::Checkbox(hiddenId.c_str(), m_value);

    ImGui::Unindent(4.0f);
    ImGui::Spacing();
  }

private:
  std::string m_label;
  bool* m_value;
};

class DropdownSetting : public ISetting {
public:
  DropdownSetting(std::string label, int* selectedIndex, std::vector<std::string> options, std::function<void()> onChange = nullptr)
    : m_label(std::move(label)), m_selectedIndex(selectedIndex), m_options(std::move(options)), m_onChange(std::move(onChange)) {}

  void draw() override {
    ImGui::Spacing();
    ImGui::Indent(4.0f);

    drawLabelAbove(m_label);
    std::string hiddenId = "##" + m_label;

    ImGui::SetNextItemWidth(-1);

    if (ImGui::BeginCombo(hiddenId.c_str(), m_options[*m_selectedIndex].c_str())) {
      for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
        const bool isSelected = (*m_selectedIndex == i);
        if (ImGui::Selectable(m_options[i].c_str(), isSelected)) {
          if (*m_selectedIndex != i) {
            *m_selectedIndex = i;
            if (m_onChange) m_onChange();
          }
        }
        if (isSelected) ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }

    ImGui::Unindent(4.0f);
    ImGui::Spacing();
  }

private:
  std::string m_label;
  int* m_selectedIndex;
  std::vector<std::string> m_options;
  std::function<void()> m_onChange;
};

#endif