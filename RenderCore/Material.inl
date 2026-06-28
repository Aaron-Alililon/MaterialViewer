#ifndef MATERIAL_INL
#define MATERIAL_INL

#include "Material.h"

// Header guards and include necessary for intellisense to work properly

namespace rcore {

  template<typename TProperties>
  Material<TProperties>::Material(Shader const& shader, int bufferSlot, uint8_t shaderStages) : m_shader{ shader }, m_bufferSlot{ bufferSlot }, m_shaderStages{ shaderStages } {
    if (m_shader.valid() &&
        createBuffer()
    ) {
      m_valid = true;
    }
  }

  template<typename TProperties>
  void Material<TProperties>::activateShader() const {
    m_shader.activate();
  }

  template<typename TProperties>
  bool Material<TProperties>::setProperties(TProperties properties, bool updateShader) {
    m_properties = properties;

    if (updateShader) return uploadProperties();
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

  template<typename TProperties>
  bool Material<TProperties>::uploadProperties() const {
    HRESULT result;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = D3D11Device::get().rawContext()->Map(m_propertiesBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to map properties buffer");
      return false;
    }

    TProperties* dataPtr = reinterpret_cast<TProperties*>(mappedResource.pData);
    *dataPtr = m_properties;

    D3D11Device::get().rawContext()->Unmap(m_propertiesBuffer.Get(), 0);

    if (m_shaderStages & ShaderStage::Vertex) D3D11Device::get().rawContext()->VSSetConstantBuffers(m_bufferSlot, 1, m_propertiesBuffer.GetAddressOf());
    if (m_shaderStages & ShaderStage::Pixel ) D3D11Device::get().rawContext()->PSSetConstantBuffers(m_bufferSlot, 1, m_propertiesBuffer.GetAddressOf());

    return true;
  }

}

#endif