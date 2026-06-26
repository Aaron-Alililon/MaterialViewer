#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include "D3D11Device.h"

namespace rcore {

  class MatrixBuffer {
  public:
    struct __declspec(align(16)) MatrixBufferType {
      DirectX::XMMATRIX world;
      DirectX::XMMATRIX view;
      DirectX::XMMATRIX projection;
      DirectX::XMMATRIX worldInverseTranspose;
    };

  public:
    MatrixBuffer();

  public:
    bool setMatrices(MatrixBufferType const& matrices, UINT startSlot = 0) const;

  private:
    bool createBuffer();
    
  private:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
  };

}

#endif