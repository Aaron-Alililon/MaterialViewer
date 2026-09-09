#ifndef LIGHT_BUFFER_H
#define LIGHT_BUFFER_H

#include "D3D11/Buffer/DBuffer.h"
#include "D3D11/Buffer/CBuffer.h"

namespace rcore {

  struct __declspec(align(16)) LightBufferType {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 direction;
    DirectX::XMFLOAT4 color;
  };

  class LightBuffer : public DBuffer<LightBufferType> {
  private:
    struct __declspec(align(16)) NumLightsBufferType {
      int numLights;
    };

  public:
    LightBuffer(int srvSlot, int numLightsBufferSlot, uint8_t shaderStages = ShaderStage::Pixel);

  public:
    void setData(std::span<LightBufferType const> data) override;
    bool uploadBuffer() override;

  private:
    CBuffer<NumLightsBufferType> m_numLightsBuffer;
  };

}

#endif