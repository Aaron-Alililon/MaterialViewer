#ifndef WINDOW_DESC_H
#define WINDOW_DESC_H

#include "pch.h"

namespace rcore {

  class WindowDesc {
  public:
    void name(LPCWSTR name);
    LPCWSTR name() const;

    void width(int width);
    int width() const;

    void height(int height);
    int height() const;

  private:
    LPCWSTR m_name = L"Window";
    int m_width = 400;
    int m_height = 400;
  };

}

#endif