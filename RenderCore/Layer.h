#ifndef LAYER_H
#define LAYER_H

#include "pch.h"
#include "FrameState.h"

namespace rcore {

  class Window; // Forward declared

  class Layer {
  public:
    Layer(std::weak_ptr<Window> const& window);

  public:
    virtual void update(FrameState const& frame) {}
    virtual void render(FrameState const& frame) {}
    virtual bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) { return true; }

    virtual bool isUI() const { return false; }

  protected:
    std::weak_ptr<Window> m_window;
  };

}

#endif