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

}