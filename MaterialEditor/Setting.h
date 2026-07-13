#ifndef SETTING_H
#define SETTING_H

#include <string>

#include "imgui.h"

class ISetting {
public:
  virtual ~ISetting() = default;
  virtual void draw() = 0;
};

class FloatSliderSetting : public ISetting {
public:
  FloatSliderSetting(std::string label, float* value, float min, float max, std::function<void()> onChange = nullptr)
    : m_label(std::move(label)), m_value(value), m_min(min), m_max(max), m_onChange{ std::move(onChange) } {}

  void draw() override {
    if (ImGui::SliderFloat(m_label.c_str(), m_value, m_min, m_max)) {
      if (m_onChange) m_onChange();
    }
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
    ImGui::Checkbox(m_label.c_str(), m_value);
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
    if (ImGui::BeginCombo(m_label.c_str(), m_options[*m_selectedIndex].c_str())) {
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
  }

private:
  std::string m_label;
  int* m_selectedIndex;
  std::vector<std::string> m_options;
  std::function<void()> m_onChange;
};

#endif