#include "pch.h"
#include "WindowView.h"

namespace rcore {

  WindowView::WindowView(Window* rawWindow) : m_window{ rawWindow } {}

  bool WindowView::isValid() const {
    return Engine::get().windowIsAlive(m_window);
  }

  Window* WindowView::raw() const {
    if (!isValid()) {
      RCORE_LOG(ERR, "Tried accessing destroyed raw window through WindowView");
      assert(false && "Check log for more information");
    }

    return m_window;
  }

}