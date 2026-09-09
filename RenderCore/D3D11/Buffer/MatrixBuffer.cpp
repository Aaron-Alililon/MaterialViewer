#include "Core/pch.h"
#include "D3D11/Buffer/MatrixBuffer.h"

namespace rcore {

  MatrixBuffer::MatrixBuffer(int bufferSlot, uint8_t shaderStages) : CBuffer{ bufferSlot, shaderStages } {}

  void MatrixBuffer::setWorldMatrix(DirectX::XMMATRIX const& worldMatrix) {
    m_data.world = worldMatrix;
  }

  void MatrixBuffer::setViewMatrix(DirectX::XMMATRIX const& viewMatrix) {
    m_data.view = viewMatrix;
  }

  void MatrixBuffer::setProjectionMatrix(DirectX::XMMATRIX const& projectionMatrix) {
    m_data.projection = projectionMatrix;
  }

  bool MatrixBuffer::uploadBuffer() const {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map matrix buffer");
      return false;
    }

    MatrixBufferType* dataPtr = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);

    dataPtr->world = XMMatrixTranspose(m_data.world);
    dataPtr->view = XMMatrixTranspose(m_data.view);
    dataPtr->projection = XMMatrixTranspose(m_data.projection);
    dataPtr->worldInverseTranspose = XMMatrixInverse(nullptr, m_data.world);

    D3D11Device::get().rawContext()->Unmap(m_buffer.Get(), 0);

    if (m_shaderStages & Vertex) D3D11Device::get().rawContext()->VSSetConstantBuffers(m_bufferSlot, 1, m_buffer.GetAddressOf());
    if (m_shaderStages & Pixel) D3D11Device::get().rawContext()->PSSetConstantBuffers(m_bufferSlot, 1, m_buffer.GetAddressOf());

    return true;
  }

}