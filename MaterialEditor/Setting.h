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
  FloatSliderSetting(std::string label, float* value, float min, float max, std::function<void(float)> onChange = nullptr)
    : m_label(std::move(label)), m_value(value), m_min(min), m_max(max), m_onChange{ std::move(onChange) } {}

  void draw() override {
    if (ImGui::SliderFloat(m_label.c_str(), m_value, m_min, m_max)) {
      if (m_onChange) m_onChange(*m_value);
    }
  }

private:
  std::string m_label;
  float* m_value;
  float m_min, m_max;
  std::function<void(float)> m_onChange;
};

class BoolSetting : public ISetting {
public:
  BoolSetting(std::string label, bool* value, std::function<void(float)> onChange)
    : m_label(std::move(label)), m_value(value) {}

  void draw() override {
    ImGui::Checkbox(m_label.c_str(), m_value);
  }

private:
  std::string m_label;
  bool* m_value;
};

#endif