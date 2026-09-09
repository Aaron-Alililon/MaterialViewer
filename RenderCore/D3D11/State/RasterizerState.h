#ifndef RASTERIZER_STATE_H
#define RASTERIZER_STATE_H

#include "D3D11/D3D11Device.h"

namespace rcore {

  class RasterizerState {
  public:
    RasterizerState() = default;
    RasterizerState(D3D11_RASTERIZER_DESC const& descriptor);

  public:
    void bind() const;
    bool isValid() const;

  private:
    bool createRasterizerState(D3D11_RASTERIZER_DESC const& descriptor);

  private:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_state;
  };

}

#endif