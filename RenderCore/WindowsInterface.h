#ifndef WINDOWS_INTERFACE_H
#define WINDOWS_INTERFACE_H

#include "pch.h"
#include "InputManager.h"
#include "Engine.h"

namespace rcore {

  class Window; // Forward declared

  class WindowsInterface {
  public:
    WindowsInterface(LPCWSTR applicationName, int screenWidth, int screenHeight);
    WindowsInterface(WindowsInterface const& other) = delete;
    WindowsInterface& operator=(WindowsInterface const& other) = delete;
    ~WindowsInterface();

  public:
    bool readMessages() const;
    LRESULT CALLBACK handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
    void hintClose() const;

  private:
    bool m_windowClosing;
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

    friend Window;
  };

}

#endif