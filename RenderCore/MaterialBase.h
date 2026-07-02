#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

#include "pch.h"

namespace rcore {

  class MaterialBase {
  public:
    virtual ~MaterialBase() = default;
    virtual void activateShader() const = 0;
    virtual bool uploadProperties() const = 0;
    virtual void uploadTextures(std::span<ID3D11ShaderResourceView*> const& textureViews, UINT startSlot) const = 0;
    virtual void uploadSamplers(std::span<ID3D11SamplerState*> const& samplerViews, UINT startSlot) const = 0;
    virtual bool valid() const = 0;
  };

}

#endif