#include "pch.h"
#include "Engine.h"
#include "Window.h"

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

      if (InputManager::get().isKeyDown(VK_ESCAPE)) {
        for (auto const& window : m_windows) {
          window->hintClose();
        }
      }

      bool anyWindowClosing = false;
      for (auto const& window : m_windows) {
        if (!window->readMessages()) {
          std::erase(m_windowAliveStates, window.get());
          anyWindowClosing = true;
        } else {
          window->update();
          window->render();
          window->endFrame();
        }
      }

      if (anyWindowClosing) {
        std::erase_if(m_windows, [&](auto const& window) {
          if (!windowIsAlive(window.get())) {
            return true;
          }
          return false;
        });
      }
    }

    shutdown();
  }

  void Engine::registerWindow(std::unique_ptr<Window> window, bool flush) {
    m_windowCreationQueue.push(std::move(window));
    if (flush) _registerWindows();
  }

  Window* Engine::getWindowByHandle(HWND hwnd) const {
    for (auto const& window : m_windows) {
      if (window->isWindowByHandle(hwnd)) return window.get();
    }

    return nullptr;
  }

  bool Engine::windowIsAlive(Window* rawWindow) const {
    return std::find(m_windowAliveStates.begin(), m_windowAliveStates.end(), rawWindow) != m_windowAliveStates.end();
  }

  bool Engine::isRunning() const {
    return m_running;
  }

  void Engine::_registerWindows() {
    while (m_windowCreationQueue.size() > 0) {
      auto window = std::move(m_windowCreationQueue.front());
      m_windowCreationQueue.pop();

      m_windowAliveStates.push_back(window.get());
      m_windows.push_back(std::move(window));
    }
  }

  void Engine::shutdown() {
    m_windows.clear();
    m_windowAliveStates.clear();
    delete instance;
    instance = nullptr;
  }

}