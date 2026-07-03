#include "pch.h"
#include "WindowsInterface.h"
#include "Window.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	auto weakWindow = rcore::Engine::get().getWindowByHandle(hwnd);
	auto window = weakWindow.lock();

	if (!window) {
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}

	return window->handleMessage(hwnd, umessage, wparam, lparam);
}

namespace rcore {

	WindowsInterface::WindowsInterface(LPCWSTR applicationName, int screenWidth, int screenHeight, std::optional<int> windowPosX, std::optional<int> windowPosY) : m_windowClosing{ false }, m_applicationName { applicationName } {
		m_hinstance = GetModuleHandle(NULL);

		WNDCLASSEX wc{};
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = m_hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = m_applicationName;
		wc.cbSize = sizeof(WNDCLASSEX);

		RegisterClassEx(&wc);

		RECT windowRect = { 0, 0, screenWidth, screenHeight };
		AdjustWindowRectEx(&windowRect, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, WS_EX_APPWINDOW);

		int adjustedWidth = windowRect.right - windowRect.left;
		int adjustedHeight = windowRect.bottom - windowRect.top;

		int posX = (windowPosX.has_value()) ? windowPosX.value() : (GetSystemMetrics(SM_CXSCREEN) - adjustedWidth) / 2;
		int posY = (windowPosY.has_value()) ? windowPosY.value() : (GetSystemMetrics(SM_CYSCREEN) - adjustedHeight) / 2;
		
		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
			WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, // | WS_THICKFRAME, // TODO Window resize event needs to resize the D3D11Context stuff
			posX, posY, adjustedWidth, adjustedHeight, NULL, NULL, m_hinstance, NULL);

		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
  }

  WindowsInterface::~WindowsInterface() {
		if (!m_windowClosing) { // If object is destroyed before WM_DESTROY is received
			DestroyWindow(m_hwnd);
		}
		m_hwnd = NULL;

		UnregisterClass(m_applicationName, m_hinstance);
		m_hinstance = NULL;
  }

	bool WindowsInterface::readMessages() const {
		if (m_windowClosing) {
			return false;
		}

		MSG msg{};

		while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return !m_windowClosing;
	}

	LRESULT CALLBACK WindowsInterface::handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
		switch (umsg) {
		case WM_DESTROY:
			m_windowClosing = true;
			return 0;

		case WM_CLOSE: // X pressed -> Let windows handle window closing sequence -> Windows throws WM_DESTROY -> Engine loop catches closing flag and destroys object
			return DefWindowProc(hwnd, umsg, wparam, lparam);

		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}

	void WindowsInterface::hintClose() const {
		PostMessage(m_hwnd, WM_CLOSE, 0, 0);
	}

}