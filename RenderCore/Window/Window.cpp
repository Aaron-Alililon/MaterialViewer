#include "Core/pch.h"
#include "Window/Window.h"

namespace rcore {

  Window::Window(WindowDesc const& descriptor) :
    m_winInterface{ descriptor.name(), descriptor.width(), descriptor.height(), descriptor.windowPosX(), descriptor.windowPosY(), descriptor.styles(), descriptor.extendedStyles() },
    m_context{ nullptr },
    m_frameState{},
    m_width{ descriptor.width() },
    m_height{ descriptor.height() },
    m_lastFrameTime{ 0 },
    m_frequency{ 0 }
  {
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_lastFrameTime);
  }

  bool Window::readMessages() const {
    return m_winInterface.readMessages();
  }

  void Window::hintClose() const {
    m_winInterface.hintClose();
  }

  bool Window::pendingClose() const {
    return m_winInterface.m_windowClosing;
  }

  void Window::startFrame() {
    if (!m_context) return;

    m_frameState.width = m_width;
    m_frameState.height = m_height;
    m_context->activate();
  }

  void Window::update() const {
    if (!m_context) return;

    for (auto const& layer : m_layers) {
      layer->update(m_frameState);
    }
  }

  void Window::render() const {
    if (!m_context) return;

    for (auto const& layer : m_layers) {
      if (!layer->isUI()) {
        layer->render(m_frameState);
      }
    }

    m_context->resolveToBackBuffer();

    for (auto const& layer : m_layers) {
      if (layer->isUI()) {
        layer->render(m_frameState);
      }
    }
  }

  void Window::endFrame() {
    if (!m_context) return;

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    m_frameState.dTime = (double)(now.QuadPart - m_lastFrameTime.QuadPart) / m_frequency.QuadPart;
    m_lastFrameTime = now;

    m_frameState.frameCount++;
    m_context->presentSwapChain();
  }

  bool Window::isWindowByHandle(HWND hwnd) const {
    return m_winInterface.m_hwnd == hwnd;
  }

  LRESULT CALLBACK Window::handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
    for (auto const& layer : m_layers) {
      if (!layer->onEvent(hwnd, umsg, wparam, lparam)) break;
    }

    return m_winInterface.handleMessage(hwnd, umsg, wparam, lparam);
  }

  void Window::setContext(std::unique_ptr<D3D11Context> context) {
    m_context = std::move(context);
  }

  void Window::activateContext() const {
    m_context->activate();
  }

  void Window::bindContextStates() const {
    m_context->bindStates();
  }

  HWND Window::getHandle() const {
    return m_winInterface.m_hwnd;
  }

  std::pair<int, int> Window::getSize() const {
    return std::make_pair(m_width, m_height);
  }

  IDXGISwapChain* Window::getSwapChain() const {
    return m_context->m_swapChain.Get();
  }

  ID3D11RenderTargetView* Window::getSceneRenderTargetView() const {
    return m_context->m_msaaRenderTargetView.Get();
  }

  ID3D11RenderTargetView* Window::getUIRenderTargetView() const {
    return m_context->m_renderTargetView.Get();
  }

  ID3D11DepthStencilView* Window::getDepthStencilView() const {
    return m_context->m_depthStencilView.Get();
  }

}