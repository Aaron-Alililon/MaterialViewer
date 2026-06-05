#ifndef WINDOW_VIEW_H
#define WINDOW_VIEW_H

#include "Window.h"

namespace rcore {

  class WindowView {
  private:
    WindowView(Window* rawWindow);

  public:
    bool isValid() const;
    Window* raw() const;

    template<std::derived_from<Layer> T>
    void addLayer() const;

  private:
    Window* m_window;

    friend WindowView makeWindow(WindowDesc const& descriptor);
    friend void makeD3D11Context(WindowView const& window, D3DContextDesc const& descriptor);
  };

}

#include "WindowView.inl"

#endif