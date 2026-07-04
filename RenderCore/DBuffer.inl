#ifndef D_BUFFER_INL
#define D_BUFFER_INL

#include "DBuffer.h"

namespace rcore {

  template<typename TData>
  DBuffer<TData>::DBuffer(int srvSlot, uint8_t shaderStages) : m_srvSlot{ srvSlot }, m_shaderStages{ shaderStages } { }

  template<typename TData>
  void DBuffer<TData>::setData(std::span<TData const> data) {
    m_data.assign(data.begin(), data.end());

    if (m_data.size() > m_capacity) {
      UINT newCapacity = m_capacity == 0 ? 4 : m_capacity;
      while (newCapacity < m_data.size()) newCapacity *= 2;
      createBuffer(newCapacity);
    }
  }

  template<typename TData>
  bool DBuffer<TData>::uploadBuffer() {
    if (!m_valid) {
      RCORE_LOG(WARN, "Tried uploading invalid dynamic buffer");
      return false;
    }

    if (m_data.empty()) return true;

    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT result = D3D11Device::get().rawContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map dynamic buffer");
      return false;
    }

    std::memcpy(mapped.pData, m_data.data(), sizeof(TData) * m_data.size());
    D3D11Device::get().rawContext()->Unmap(m_buffer.Get(), 0);

    if (m_shaderStages & ShaderStage::Vertex) D3D11Device::get().rawContext()->VSSetShaderResources(m_srvSlot, 1, m_srv.GetAddressOf());
    if (m_shaderStages & ShaderStage::Pixel) D3D11Device::get().rawContext()->PSSetShaderResources(m_srvSlot, 1, m_srv.GetAddressOf());

    return true;
  }

  template<typename TData>
  bool DBuffer<TData>::createBuffer(UINT capacity) {
    D3D11_BUFFER_DESC desc{};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(TData) * capacity;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    desc.StructureByteStride = sizeof(TData);

    HRESULT result = D3D11Device::get().raw()->CreateBuffer(&desc, nullptr, &m_buffer);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create dynamic buffer");
      m_valid = false;
      return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    srvDesc.BufferEx.FirstElement = 0;
    srvDesc.BufferEx.NumElements = capacity;

    result = D3D11Device::get().raw()->CreateShaderResourceView(m_buffer.Get(), &srvDesc, &m_srv);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create dynamic buffer SRV");
      m_valid = false;
      return false;
    }

    m_capacity = capacity;
    m_valid = true;
    return true;
  }

}

#endif