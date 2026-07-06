#ifndef DEPTH_STENCIL_STATE_H
#define DEPTH_STENCIL_STATE_H

#include "D3D11Device.h"

namespace rcore {

  class DepthStencilState {
  public:
    DepthStencilState() = default;
    DepthStencilState(D3D11_DEPTH_STENCIL_DESC const& descriptor);

  public:
    void bind() const;
    bool isValid() const;

  private:
    bool createDepthStencilState(D3D11_DEPTH_STENCIL_DESC const& descriptor);

  private:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_state;
  };

}

#endif