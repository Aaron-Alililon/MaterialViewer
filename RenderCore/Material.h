#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialBase.h"
#include "Shader.h"
#include "ShaderStage.h"

namespace rcore {

  template<typename TProperties>
  class Material : public MaterialBase {
  public:
    Material() = default;
    Material(Shader const& shader, int bufferSlot, uint8_t shaderStages = ShaderStage::Pixel);

  public:
    bool setProperties(TProperties properties, bool updateShader = true);
    bool uploadProperties() const override;
    void activateShader() const override;
    bool valid() const override;

  private:
    bool createBuffer();

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