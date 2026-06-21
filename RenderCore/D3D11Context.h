#ifndef D3D11_CONTEXT_H
#define D3D11_CONTEXT_H

#include "D3DContextDesc.h"
#include "D3D11Device.h"

namespace rcore {

  class Window; // Forward declared

  class D3D11Context {
  public:
    D3D11Context(std::shared_ptr<Window> const& window, D3DContextDesc const& descriptor);

  public:
    void activate();
    void presentSwapChain() const;

  private:
    bool createSwapChain(std::pair<int, int> windowSize, HWND windowHandle, int targetFps);
    bool createRenderTargetView();
    bool createDepthBuffer(D3D11_TEXTURE2D_DESC const& bufferDescriptor, D3D11_DEPTH_STENCIL_DESC const& stencilDescriptor, D3D11_DEPTH_STENCIL_VIEW_DESC const& stencilViewDescriptor);
    bool createRasterState(D3D11_RASTERIZER_DESC const& rasterDescriptor);
    bool createViewport(std::pair<int, int> const& screenSize);

  private:
    bool m_valid;

    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterState;
    D3D11_VIEWPORT m_viewport;

    friend Window;
  };

}

#endif