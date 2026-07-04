#ifndef C_BUFFER_H
#define C_BUFFER_H

#include "pch.h"
#include "D3D11Device.h"

namespace rcore {

  template<typename TBuffer>
  class CBuffer {
  public:
    CBuffer(int bufferSlot);
    virtual ~CBuffer() = default;

  public:
    virtual void setData(TBuffer const& data);
    virtual bool uploadBuffer() const;

  private:
    virtual bool createBuffer();

  protected:
    bool m_valid = false;
    TBuffer m_data{};
    int m_bufferSlot = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
  };

}

#include "CBuffer.inl"

#endif