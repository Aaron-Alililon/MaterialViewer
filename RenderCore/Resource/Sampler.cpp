#include "Core/pch.h"
#include "Resource/Sampler.h"

namespace rcore {

  Sampler::Sampler(D3D11_SAMPLER_DESC const& descriptor) {
    if (loadSampler(descriptor)) {
      m_valid = true;
    }
  }

  ID3D11SamplerState* Sampler::getSamplerState() const {
    return m_samplerState.Get();
  }

  bool Sampler::loadSampler(D3D11_SAMPLER_DESC const& descriptor) {
    HRESULT result = D3D11Device::get().raw()->CreateSamplerState(&descriptor, m_samplerState.GetAddressOf());

    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to load sampler state");
      return false;
    }

    return true;
  }

}