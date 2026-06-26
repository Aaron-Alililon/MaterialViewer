#ifndef WINDOW_H
#define WINDOW_H

#include "WindowDesc.h"
#include "Layer.h"
#include "Engine.h"
#include "WindowsInterface.h"
#include "D3D11Context.h"

namespace rcore {

  class Window : public std::enable_shared_from_this<Window> {
  public:
    Window(WindowDesc const& descriptor);

  public:
    bool readMessages() const;
    void hintClose() const;
    bool pendingClose() const;
    void startFrame();
    void update() const;
    void render() const;
    void endFrame();
    bool isWindowByHandle(HWND hwnd) const;
    LRESULT CALLBACK handleMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
    void setContext(std::unique_ptr<D3D11Context> context);

    HWND getHandle() const;
    std::pair<int, int> getSize() const;
    IDXGISwapChain* getSwapChain() const;
    ID3D11RenderTargetView* getRenderTargetView() const;
    ID3D11DepthStencilView* getDepthStencilView() const;

    template<std::derived_from<Layer> T, class... Args>
    void addLayer(Args&&... args);

  private:
    WindowsInterface m_winInterface;
    std::unique_ptr<D3D11Context> m_context;
    std::vector<std::unique_ptr<Layer>> m_layers;
    FrameState m_frameState;
    int m_width, m_height;
    LARGE_INTEGER m_lastFrameTime;
    LARGE_INTEGER m_frequency;
  };

}

#include "Window.inl"

#endif