#ifndef D_BUFFER_H
#define D_BUFFER_H

#include "Core/pch.h"
#include "D3D11/D3D11Device.h"
#include "Render/ShaderStage.h"

namespace rcore {

  template<typename TData>
  class DBuffer {
  public:
    DBuffer(int srvSlot, uint8_t shaderStages);
    virtual ~DBuffer() = default;

  public:
    virtual void setData(std::span<TData const> data);
    virtual bool uploadBuffer();

  private:
    virtual bool createBuffer(UINT capacity);

  protected:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    UINT m_capacity = 0;
    int m_srvSlot;
    uint8_t m_shaderStages;
    std::vector<TData> m_data;
  };

}

#include "D3D11/Buffer/DBuffer.inl"

#endif