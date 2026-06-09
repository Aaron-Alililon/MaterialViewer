#include "pch.h"
#include "Window.h"

namespace rcore {

  Window::Window(WindowDesc const& descriptor) :
    m_winInterface{ descriptor.name(), descriptor.width(), descriptor.height() },
    m_context{ nullptr },
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

  bool Window::pendingClose() const {
    return m_winInterface.m_windowClosing;
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

  void Window::setContext(std::unique_ptr<D3D11Context> context) {
    m_context = std::move(context);
  }

  HWND Window::getHandle() const {
    return m_winInterface.m_hwnd;
  }

  std::pair<int, int> Window::getSize() const {
    return std::make_pair(m_width, m_height);
  }

}