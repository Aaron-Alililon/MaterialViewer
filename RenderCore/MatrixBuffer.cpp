#include "pch.h"
#include "MatrixBuffer.h"

namespace rcore {

  MatrixBuffer::MatrixBuffer() {
    if (createBuffer()) {
      m_valid = true;
    }
  }

  bool MatrixBuffer::setMatrices(MatrixBufferType const& matrices, UINT startSlot) const {
    HRESULT result;

    DirectX::XMMATRIX worldMatrix = XMMatrixTranspose(matrices.world);
    DirectX::XMMATRIX viewMatrix = XMMatrixTranspose(matrices.view);
    DirectX::XMMATRIX projectionMatrix = XMMatrixTranspose(matrices.projection);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map matrix buffer");
      return false;
    }

    MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

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