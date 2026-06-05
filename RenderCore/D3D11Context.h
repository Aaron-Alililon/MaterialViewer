#ifndef D3D11_CONTEXT_H
#define D3D11_CONTEXT_H

#include "D3DContextDesc.h"
#include "D3D11Device.h"

namespace rcore {

  class WindowView; // Forward declared

  class D3D11Context {
  public:
    D3D11Context(WindowView const& window, D3DContextDesc const& descriptor);

  private:
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
  };

}

#endif