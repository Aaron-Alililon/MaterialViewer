#ifndef WINDOW_H
#define WINDOW_H

#include "WindowDesc.h"
#include "Layer.h"
#include "Engine.h"
#include "WindowsInterface.h"

namespace rcore {

  class Window {
  public:
    Window(WindowDesc const& descriptor);

  public:
    bool readMessages() const;
    void hintClose() const;
    void update() const;
    void render() const;
    void endFrame();
    bool isWindowByHandle(HWND hwnd) const;
    LRESULT CALLBACK handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

    template<std::derived_from<Layer> T>
    void addLayer();

  private:
    WindowsInterface m_winInterface;
    std::vector<std::unique_ptr<Layer>> m_layers;
    FrameState m_frameState;
    int m_width, m_height;
  };

}

#include "Window.inl"

#endif