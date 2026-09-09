#ifndef MATRIX_BUFFER_H
#define MATRIX_BUFFER_H

#include "D3D11/Buffer/CBuffer.h"
#include "D3D11/D3D11Device.h"

namespace rcore {

  struct __declspec(align(16)) MatrixBufferType {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX worldInverseTranspose;
  };

  class MatrixBuffer : public CBuffer<MatrixBufferType> {
  public:
    MatrixBuffer(int bufferSlot, uint8_t shaderStages = ShaderStage::Vertex);

  public:
    void setWorldMatrix(DirectX::XMMATRIX const& worldMatrix);
    void setViewMatrix(DirectX::XMMATRIX const& viewMatrix);
    void setProjectionMatrix(DirectX::XMMATRIX const& projectionMatrix);
    bool uploadBuffer() const override;
  };

}

#endif