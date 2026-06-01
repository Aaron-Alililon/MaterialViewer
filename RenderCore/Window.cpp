#include "pch.h"
#include "Window.h"

namespace rcore {

  Window::Window(WindowDesc const& descriptor) :
    m_winInterface{ descriptor.name(), descriptor.width(), descriptor.height() },
    m_frameState{},
    m_width{ descriptor.width() },
    m_height{ descriptor.height() }
    {}

  bool Window::readMessages() const {
    return m_winInterface.readMessages();
  }

  void Window::hintClose() const {
    m_winInterface.hintClose();
  }

  void Window::update() const {
    for (auto const& layer : m_layers) {
      layer->update(m_frameState);
    }
  }

  void Window::render() const {
    for (auto const& layer : m_layers) {
      layer->render(m_frameState);
    }
  }

  void Window::endFrame() {
    m_frameState.frameCount++;
  }

  bool Window::isWindowByHandle(HWND hwnd) const {
    return m_winInterface.m_hwnd == hwnd;
  }

  LRESULT CALLBACK Window::handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    return m_winInterface.handleMessage(hwnd, umsg, wparam, lparam);
  }

}