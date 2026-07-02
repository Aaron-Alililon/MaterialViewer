#ifndef SAMPLER_H
#define SAMPLER_H

#include "pch.h"
#include "D3D11Device.h"

namespace rcore {

  class Sampler {
  public:
    Sampler(D3D11_SAMPLER_DESC const& descriptor);

  public:
    ID3D11SamplerState* getSamplerState() const;

  private:
    bool loadSampler(D3D11_SAMPLER_DESC const& descriptor);

  private:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
  };

}

#endif