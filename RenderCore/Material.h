#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "ShaderStage.h"

namespace rcore {

  template<typename TProperties>
  class Material {
  public:
    Material() = default;
    Material(Shader const& shader, int bufferSlot, uint8_t shaderStages = ShaderStage::Pixel);

  public:
    void activateShader() const;
    bool setProperties(TProperties properties, bool updateShader = true);

  private:
    bool createBuffer();
    bool uploadProperties() const;

  private:
    bool m_valid = false;
    Shader m_shader{};
    TProperties m_properties{};
    int m_bufferSlot = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_propertiesBuffer;
    uint8_t m_shaderStages = 0;
  };

}

#include "Material.inl"

#endif