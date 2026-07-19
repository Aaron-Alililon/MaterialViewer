#include "pch.h"
#include "WindowDesc.h"

namespace rcore {

  void WindowDesc::name(LPCWSTR name) {
    m_name = name;
  }

  LPCWSTR WindowDesc::name() const {
    return m_name;
  }

  void WindowDesc::width(int width) {
    assert(width > 0 && "Window width has to be greater than 0");
    m_width = width;
  }

  int WindowDesc::width() const {
    return m_width;
  }

  void WindowDesc::height(int height) {
    assert(height > 0 && "Window height has to be greater than 0");
    m_height = height;
  }

  int WindowDesc::height() const {
    return m_height;
  }

  void WindowDesc::windowPosX(std::optional<int> x) {
    m_windowPosX = x;
  }

  std::optional<int> WindowDesc::windowPosX() const {
    return m_windowPosX;
  }

  void WindowDesc::windowPosY(std::optional<int> y) {
    m_windowPosY = y;
  }

  std::optional<int> WindowDesc::windowPosY() const {
    return m_windowPosY;
  }

  void WindowDesc::toggleStyle(DWORD style) {
    m_styles ^= style;
  }

  DWORD WindowDesc::styles() const {
    return m_styles;
  }

  void WindowDesc::toggleExtendedStyle(DWORD exStyle) {
    m_extendedStyles ^= exStyle;
  }

  DWORD WindowDesc::extendedStyles() const {
    return m_extendedStyles;
  }

}