#include "Core/pch.h"
#include "Core/Engine.h"
#include "Window/Window.h"

namespace rcore {

  Engine* Engine::instance = nullptr;

  Engine& Engine::get() {
    if (instance == nullptr) {
      instance = new Engine();
    }

    return *instance;
  }

  void Engine::run() {
    m_running = true;

    while (!m_windows.empty() || !m_windowCreationQueue.empty()) {
      _registerWindows();

      bool anyWindowClosing = false;
      for (auto const& window : m_windows) {
        if (!window->readMessages()) {
          anyWindowClosing = true;
        } else {
          window->startFrame();
          window->update();
          window->render();
          window->endFrame();
        }
      }

      if (anyWindowClosing) {
        std::erase_if(m_windows, [&](auto const& window) {
          return window->pendingClose();
        });
      }
    }

    shutdown();
  }

  void Engine::registerWindow(std::shared_ptr<Window> window, bool flush) {
    m_windowCreationQueue.push(std::move(window));
    if (flush) _registerWindows();
  }

  std::weak_ptr<Window> Engine::getWindowByHandle(HWND hwnd) const {
    for (auto const& window : m_windows) {
      if (window->isWindowByHandle(hwnd)) return window;
    }

    return {};
  }

  bool Engine::isRunning() const {
    return m_running;
  }

  void Engine::_registerWindows() {
    while (m_windowCreationQueue.size() > 0) {
      auto window = std::move(m_windowCreationQueue.front());
      m_windowCreationQueue.pop();
      m_windows.push_back(std::move(window));
    }
  }

  void Engine::shutdown() {
    m_windows.clear();
    delete instance;
    instance = nullptr;
  }

}