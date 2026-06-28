#ifndef MATERIAL_BASE_H
#define MATERIAL_BASE_H

namespace rcore {

  class MaterialBase {
  public:
    virtual ~MaterialBase() = default;
    virtual void activateShader() const = 0;
    virtual bool uploadProperties() const = 0;
    virtual bool valid() const = 0;
  };

}

#endif