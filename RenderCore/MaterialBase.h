#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

#include "pch.h"

namespace rcore {

  class MaterialBase {
  public:
    virtual ~MaterialBase() = default;
    virtual void activateProperties() const = 0;
    virtual void activateTextures() const = 0;
    virtual void activateSamplers() const = 0;
    virtual void activateShader() const = 0;
    virtual void activate() const = 0;
    virtual bool valid() const = 0;
  };

}

#endif