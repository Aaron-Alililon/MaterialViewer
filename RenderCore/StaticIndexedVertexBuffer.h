#ifndef STATIC_INDEXED_VERTEX_BUFFER_H
#define STATIC_INDEXED_VERTEX_BUFFER_H

#include "D3D11Device.h"
#include "VertexBufferBase.h"

namespace rcore {

  template<typename TVertex>
  class StaticIndexedVertexBuffer : public VertexBufferBase {
  public:
    StaticIndexedVertexBuffer() = default;
    StaticIndexedVertexBuffer(std::vector<TVertex> const& vertices, std::vector<UINT> const& indices);

  public:
    UINT bind() const override;
    bool valid() const override;

  private:
    bool createVertexBuffer(std::vector<TVertex> const& vertices);
    bool createIndexBuffer(std::vector<UINT> const& indices);

  private:
    bool m_valid = false;
    UINT m_indexAmount = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
  };

}

#include "StaticIndexedVertexBuffer.inl"

#endif