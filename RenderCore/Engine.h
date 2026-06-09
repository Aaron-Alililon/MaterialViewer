#ifndef ENGINE_H
#define ENGINE_H

#include "pch.h"

namespace rcore {

  class Window; // Forward declared
  
  class Engine {
  private:
    static Engine* instance;

  public:
    static Engine& get();

  private:
    Engine() = default;

  public:
    void run();

    void registerWindow(std::shared_ptr<Window> window, bool flush = false);
    std::weak_ptr<Window> getWindowByHandle(HWND hwnd) const;
    bool isRunning() const;

  private:
    void _registerWindows();
    void shutdown();

  private:
    bool m_running = false;
    std::queue<std::shared_ptr<Window>> m_windowCreationQueue;
    std::vector<std::shared_ptr<Window>> m_windows;
  };

}

#endif