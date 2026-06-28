#include "pch.h"
#include "MatrixBuffer.h"

namespace rcore {

  MatrixBuffer::MatrixBuffer() {
    if (createBuffer()) {
      m_valid = true;
    }
  }

  void MatrixBuffer::setWorldMatrix(DirectX::XMMATRIX const& worldMatrix) {
    m_matrices.world = worldMatrix;
  }

  void MatrixBuffer::setViewMatrix(DirectX::XMMATRIX const& viewMatrix) {
    m_matrices.view = viewMatrix;
  }

  void MatrixBuffer::setProjectionMatrix(DirectX::XMMATRIX const& projectionMatrix) {
    m_matrices.projection = projectionMatrix;
  }

  void MatrixBuffer::setMatrices(MatrixBufferType const& matrices) {
    m_matrices = matrices;
  }


  bool MatrixBuffer::uploadMatrices(UINT startSlot) const {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map matrix buffer");
      return false;
    }

    MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

    dataPtr->world = XMMatrixTranspose(m_matrices.world);
    dataPtr->view = XMMatrixTranspose(m_matrices.view);
    dataPtr->projection = XMMatrixTranspose(m_matrices.projection);
    dataPtr->worldInverseTranspose = XMMatrixInverse(nullptr, m_matrices.world);

    D3D11Device::get().rawContext()->Unmap(m_matrixBuffer.Get(), 0);

    D3D11Device::get().rawContext()->VSSetConstantBuffers(startSlot, 1, m_matrixBuffer.GetAddressOf());

    return true;
  }

  bool MatrixBuffer::createBuffer() {
    HRESULT result;

    D3D11_BUFFER_DESC matrixBufferDesc{};
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    result = D3D11Device::get().raw()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create matrix buffer");
      return false;
    }

    return true;
  }

}