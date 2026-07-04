#include "pch.h"
#include "LightBuffer.h"

namespace rcore {

  LightBuffer::LightBuffer(int srvSlot, int numLightsBufferSlot, uint8_t shaderStages) : DBuffer{ srvSlot, shaderStages }, m_numLightsBuffer{ numLightsBufferSlot, shaderStages } { }

  void LightBuffer::setData(std::span<LightBufferType const> data) {
    DBuffer::setData(data);
    m_numLightsBuffer.setData({ static_cast<int>(data.size()) });
  }

  bool LightBuffer::uploadBuffer() {
    if (!DBuffer::uploadBuffer()) return false;
    return m_numLightsBuffer.uploadBuffer();
  }

}