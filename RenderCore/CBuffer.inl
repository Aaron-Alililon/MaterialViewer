#ifndef C_BUFFER_INL
#define C_BUFFER_INL

#include "CBuffer.h"

namespace rcore {

  template<typename TBuffer>
  CBuffer<TBuffer>::CBuffer(int bufferSlot) : m_bufferSlot{ bufferSlot } {
    if (createBuffer()) {
      m_valid = true;
    }
  }

  template<typename TBuffer>
  void CBuffer<TBuffer>::setData(TBuffer const& data) {
    m_data = data;
  }

  template<typename TBuffer>
  bool CBuffer<TBuffer>::uploadBuffer() const {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map constant buffer");
      return false;
    }

    TBuffer* dataPtr = reinterpret_cast<TBuffer*>(mappedResource.pData);

    *dataPtr = m_data;

    D3D11Device::get().rawContext()->Unmap(m_buffer.Get(), 0);

    D3D11Device::get().rawContext()->VSSetConstantBuffers(m_bufferSlot, 1, m_buffer.GetAddressOf());

    return true;
  }

  template<typename TBuffer>
  bool CBuffer<TBuffer>::createBuffer() {
    HRESULT result;

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(TBuffer);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    result = D3D11Device::get().raw()->CreateBuffer(&bufferDesc, NULL, &m_buffer);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create constant buffer");
      return false;
    }

    return true;
  }

}

#endif