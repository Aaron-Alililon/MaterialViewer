#ifndef STATIC_INDEXED_VERTEX_BUFFER_INL
#define STATIC_INDEXED_VERTEX_BUFFER_INL

#include "D3D11/Buffer/StaticIndexedVertexBuffer.h"

// Header guards and include necessary for intellisense to work properly

namespace rcore {

  template<typename TVertex>
  StaticIndexedVertexBuffer<TVertex>::StaticIndexedVertexBuffer(std::vector<TVertex> const& vertices, std::vector<UINT> const& indices, bool keepCPUData) {
    if (keepCPUData) {
      m_vertices = vertices;
      m_indices = indices;
    }

    m_indexAmount = static_cast<UINT>(indices.size());

    if (createVertexBuffer(vertices) &&
        createIndexBuffer(indices)
    ) {
      m_valid = true;
    }
  }

  template<typename TVertex>
  UINT StaticIndexedVertexBuffer<TVertex>::bind() const {
    if (!m_valid) {
      RCORE_LOG(ERR, "Tried binding invalid SIVB");
      return 0;
    }

    UINT stride = sizeof(TVertex);
    UINT offset = 0;

    D3D11Device::get().rawContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    D3D11Device::get().rawContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    return m_indexAmount;
  }

  template<typename TVertex>
  bool StaticIndexedVertexBuffer<TVertex>::valid() const {
    return m_valid;
  }

  template<typename TVertex>
  std::vector<TVertex> StaticIndexedVertexBuffer<TVertex>::getVertices() const {
    return m_vertices;
  }

  template<typename TVertex>
  std::vector<UINT> StaticIndexedVertexBuffer<TVertex>::getIndices() const {
    return m_indices;
  }

  template<typename TVertex>
  bool StaticIndexedVertexBuffer<TVertex>::createVertexBuffer(std::vector<TVertex> const& vertices) {
    HRESULT result;

    D3D11_BUFFER_DESC vertexBufferDesc{};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(TVertex) * static_cast<UINT>(vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA vertexData{};
    vertexData.pSysMem = vertices.data();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = D3D11Device::get().raw()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create static vertex buffer");
      return false;
    }

    return true;
  }

  template<typename TVertex>
  bool StaticIndexedVertexBuffer<TVertex>::createIndexBuffer(std::vector<UINT> const& indices) {
    HRESULT result;

    D3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(UINT) * static_cast<UINT>(indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA indexData{};
    indexData.pSysMem = indices.data();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = D3D11Device::get().raw()->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create static index buffer");
      return false;
    }

    return true;
  }

}

#endif