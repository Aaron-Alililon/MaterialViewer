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

    void windowPosX(std::optional<int> x);
    std::optional<int> windowPosX() const;

    void windowPosY(std::optional<int> y);
    std::optional<int> windowPosY() const;

  private:
    LPCWSTR m_name = L"Window";
    int m_width = 400;
    int m_height = 400;
    std::optional<int> m_windowPosX{};
    std::optional<int> m_windowPosY{};

  };

}

#endif