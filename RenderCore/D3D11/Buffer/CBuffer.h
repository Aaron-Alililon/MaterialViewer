#ifndef C_BUFFER_H
#define C_BUFFER_H

#include "Core/pch.h"
#include "D3D11/D3D11Device.h"
#include "Render/ShaderStage.h"

namespace rcore {

  template<typename TBuffer>
  class CBuffer {
  public:
    CBuffer(int bufferSlot, uint8_t shaderStages);
    virtual ~CBuffer() = default;

  public:
    virtual void setData(TBuffer const& data);
    virtual bool uploadBuffer() const;

  private:
    virtual bool createBuffer();

  protected:
    bool m_valid = false;
    uint8_t m_shaderStages;
    TBuffer m_data{};
    int m_bufferSlot;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
  };

}

#include "D3D11/Buffer/CBuffer.inl"

#endif