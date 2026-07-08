#include "pch.h"
#include "RenderTarget.h"

namespace rcore {

  RenderTarget::RenderTarget(D3D11_TEXTURE2D_DESC const& textureDescriptor) {
    if (createTexture(textureDescriptor)) {
      m_valid = true;
    }
  }

  void RenderTarget::clearRTV(float r, float g, float b, float a) const {
    float clearCol[] = { r, g, b, a };
    D3D11Device::get().rawContext()->ClearRenderTargetView(m_rtv.Get(), clearCol);
  }

  ID3D11Texture2D* RenderTarget::getTexture() const {
    if (!m_valid) {
      RCORE_LOG(WARN, "Tried accessing texture of invalid render target");
      return nullptr;
    }

    return m_texture.Get();
  }

  ID3D11ShaderResourceView* RenderTarget::getSRV() const {
    if (!m_valid) {
      RCORE_LOG(WARN, "Tried accessing SRV of invalid render target");
      return nullptr;
    }

    return m_srv.Get();
  }

  ID3D11RenderTargetView* RenderTarget::getRTV() const {
    if (!m_valid) {
      RCORE_LOG(WARN, "Tried accessing RTV of invalid render target");
      return nullptr;
    }

    return m_rtv.Get();
  }

  bool RenderTarget::isValid() const {
    return m_valid;
  }

  bool RenderTarget::createTexture(D3D11_TEXTURE2D_DESC const& textureDescriptor) {
    HRESULT result;
    
    result = D3D11Device::get().raw()->CreateTexture2D(&textureDescriptor, nullptr, &m_texture);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create render target texture");
      return false;
    }

    result = D3D11Device::get().raw()->CreateRenderTargetView(m_texture.Get(), nullptr, &m_rtv);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create render target RTV");
      return false;
    }

    result = D3D11Device::get().raw()->CreateShaderResourceView(m_texture.Get(), nullptr, &m_srv);
    if (FAILED(result)) {
      RCORE_LOG(ERR, "Failed to create render target SRV");
      return false;
    }

    return true;
  }

}