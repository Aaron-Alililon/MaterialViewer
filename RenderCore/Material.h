#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialBase.h"
#include "Shader.h"
#include "ShaderStage.h"

namespace rcore {

  template<typename TProperties>
  class Material : public MaterialBase {
  public:
    Material(Shader const& shader, int bufferSlot, uint8_t shaderStages = ShaderStage::Pixel);

  public:
    bool setProperties(TProperties properties, bool updateShader = true);
    bool uploadProperties() const override;
    void uploadTextures(std::span<ID3D11ShaderResourceView*> const& textureViews, UINT startSlot) const override;
    void uploadSamplers(std::span<ID3D11SamplerState*> const& samplerViews, UINT startSlot) const override;
    void activateShader() const override;
    bool valid() const override;

  private:
    bool createBuffer();

  private:
    bool m_valid = false;
    Shader m_shader;
    TProperties m_properties;
    int m_bufferSlot;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_propertiesBuffer;
    uint8_t m_shaderStages;
  };

}

#include "Material.inl"

#endif