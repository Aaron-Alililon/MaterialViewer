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
    MatrixBuffer(int bufferSlot);

  public:
    void setWorldMatrix(DirectX::XMMATRIX const& worldMatrix);
    void setViewMatrix(DirectX::XMMATRIX const& viewMatrix);
    void setProjectionMatrix(DirectX::XMMATRIX const& projectionMatrix);
    void setMatrices(MatrixBufferType const& matrices);
    bool uploadMatrices() const;

  private:
    bool createBuffer();
    
  private:
    bool m_valid = false;
    MatrixBufferType m_matrices{};
    int m_bufferSlot = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
  };

}

#endif