#ifndef MATERIAL_H
#define MATERIAL_H

#include "MaterialBase.h"
#include "Shader.h"
#include "ShaderStage.h"

namespace rcore {

  template<typename TProperties>
  class Material : public MaterialBase {
  public:
    Material(Shader const& shader, uint8_t shaderStages = ShaderStage::Pixel);

  public:
    bool uploadProperties(TProperties const& properties, std::optional<UINT> startSlot = { });
    void setTextures(std::span<ID3D11ShaderResourceView*> const& textureViews, std::optional<UINT> startSlot = { });
    void setSamplers(std::span<ID3D11SamplerState*> const& samplerViews, std::optional<UINT> startSlot = { });

    void activateProperties() const override;
    void activateTextures() const override;
    void activateSamplers() const override;
    void activateShader() const override;
    void activate() const override;

    bool valid() const override;

  private:
    bool validateShaderStages() const;
    bool createBuffer();

  private:
    bool m_valid = false;
    Shader m_shader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_propertiesBuffer;
    std::vector<ID3D11ShaderResourceView*> m_textureViews{};
    std::vector<ID3D11SamplerState*> m_samplerViews{};
    UINT m_propertiesSlot = 0;
    UINT m_texturesSlot = 0;
    UINT m_samplersSlot = 0;
    uint8_t m_shaderStages;
  };

}

#include "Material.inl"

#endif