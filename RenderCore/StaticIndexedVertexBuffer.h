#ifndef STATIC_INDEXED_VERTEX_BUFFER_H
#define STATIC_INDEXED_VERTEX_BUFFER_H

#include "D3D11Device.h"
#include "IVertexBuffer.h"

namespace rcore {

  template<typename TVertex>
  class StaticIndexedVertexBuffer : public IVertexBuffer {
  public:
    StaticIndexedVertexBuffer() = default;
    StaticIndexedVertexBuffer(std::vector<TVertex> const& vertices, std::vector<UINT> const& indices, bool keepCPUData = false);

  public:
    UINT bind() const override;
    bool valid() const override;

    std::vector<TVertex> getVertices() const;
    std::vector<UINT> getIndices() const;

  private:
    bool createVertexBuffer(std::vector<TVertex> const& vertices);
    bool createIndexBuffer(std::vector<UINT> const& indices);

  private:
    bool m_valid = false;
    UINT m_indexAmount = 0;
    std::vector<TVertex> m_vertices;
    std::vector<UINT> m_indices;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
  };

}

#include "StaticIndexedVertexBuffer.inl"

#endif