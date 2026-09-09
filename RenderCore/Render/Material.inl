#ifndef MATERIAL_INL
#define MATERIAL_INL

#include "Render/Material.h"

// Header guards and include necessary for intellisense to work properly

namespace rcore {

  template<typename TProperties>
  Material<TProperties>::Material(Shader const& shader, uint8_t shaderStages) : m_shader{ shader }, m_shaderStages{ shaderStages } {
    if (m_shader.valid() &&
        createBuffer()
    ) {
      m_valid = true;
    }
  }

  template<typename TProperties>
  bool Material<TProperties>::uploadProperties(TProperties const& properties, std::optional<UINT> startSlot) {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_propertiesBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map properties buffer");
      return false;
    }

    TProperties* dataPtr = reinterpret_cast<TProperties*>(mappedResource.pData);
    *dataPtr = properties;

    D3D11Device::get().rawContext()->Unmap(m_propertiesBuffer.Get(), 0);

    m_propertiesSlot = (startSlot.has_value()) ? startSlot.value() : m_propertiesSlot;

    return true;
  }

  template<typename TProperties>
  void Material<TProperties>::setTextures(std::span<ID3D11ShaderResourceView*> const& textureViews, std::optional<UINT> startSlot) {
    m_textureViews.assign(textureViews.begin(), textureViews.end());
    m_texturesSlot = startSlot.value_or(m_texturesSlot);
  }

  template<typename TProperties>
  void Material<TProperties>::setSamplers(std::span<ID3D11SamplerState*> const& samplerViews, std::optional<UINT> startSlot) {
    m_samplerViews.assign(samplerViews.begin(), samplerViews.end());
    m_samplersSlot = startSlot.value_or(m_samplersSlot);
  }

  template<typename TProperties>
  void Material<TProperties>::activateProperties() const {
    if (!validateShaderStages()) return;
    if (!m_propertiesBuffer) return;
    if (m_shaderStages & ShaderStage::Vertex) D3D11Device::get().rawContext()->VSSetConstantBuffers(m_propertiesSlot, 1, m_propertiesBuffer.GetAddressOf());
    if (m_shaderStages & ShaderStage::Pixel) D3D11Device::get().rawContext()->PSSetConstantBuffers(m_propertiesSlot, 1, m_propertiesBuffer.GetAddressOf());
  }

  template<typename TProperties>
  void Material<TProperties>::activateTextures() const {
    if (!validateShaderStages()) return;
    if (m_textureViews.size() == 0) return;
    if (m_shaderStages & ShaderStage::Vertex) D3D11Device::get().rawContext()->VSSetShaderResources(m_texturesSlot, static_cast<UINT>(m_textureViews.size()), m_textureViews.data());
    if (m_shaderStages & ShaderStage::Pixel) D3D11Device::get().rawContext()->PSSetShaderResources(m_texturesSlot, static_cast<UINT>(m_textureViews.size()), m_textureViews.data());
  }

  template<typename TProperties>
  void Material<TProperties>::activateSamplers() const {
    if (!validateShaderStages()) return;
    if (m_samplerViews.size() == 0) return;
    if (m_shaderStages & ShaderStage::Vertex) D3D11Device::get().rawContext()->VSSetSamplers(m_samplersSlot, static_cast<UINT>(m_samplerViews.size()), m_samplerViews.data());
    if (m_shaderStages & ShaderStage::Pixel) D3D11Device::get().rawContext()->PSSetSamplers(m_samplersSlot, static_cast<UINT>(m_samplerViews.size()), m_samplerViews.data());
  }

  template<typename TProperties>
  void Material<TProperties>::activateShader() const {
    if (!validateShaderStages()) return;
    m_shader.activate();
  }

  template<typename TProperties>
  void Material<TProperties>::activate() const {
    if (!validateShaderStages()) return;
    activateProperties();
    activateTextures();
    activateSamplers();
    activateShader();
  }

  template<typename TProperties>
  bool Material<TProperties>::valid() const {
    return m_valid;
  }

  template<typename TProperties>
  bool Material<TProperties>::validateShaderStages() const {
    if (m_shaderStages == 0) {
      RCORE_LOG(WARN, "Tried using a material with no shader stage set");
      return false;
    }

    return true;
  }

  template<typename TProperties>
  bool Material<TProperties>::createBuffer() {
    HRESULT result;

    if (sizeof(TProperties) % 16 != 0) {
      RCORE_LOG(ERR, "Tried creating material properties with sizeof(TProperties) non-multiple of 16");
      return false;
    }

    D3D11_BUFFER_DESC propertiesBufferDesc{};
    propertiesBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    propertiesBufferDesc.ByteWidth = sizeof(TProperties);
    propertiesBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    propertiesBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    propertiesBufferDesc.MiscFlags = 0;
    propertiesBufferDesc.StructureByteStride = 0;

    result = D3D11Device::get().raw()->CreateBuffer(&propertiesBufferDesc, NULL, &m_propertiesBuffer);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create properties buffer");
      return false;
    }

    return true;
  }

}

#endif