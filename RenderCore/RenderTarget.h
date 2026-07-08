#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

#include "D3D11Device.h"

namespace rcore {

  class RenderTarget {
  public:
    RenderTarget(D3D11_TEXTURE2D_DESC const& textureDescriptor);

  public:
    void clearRTV(float r = 0, float g = 0, float b = 0, float a = 0) const;

    ID3D11Texture2D* getTexture() const;
    ID3D11ShaderResourceView* getSRV() const;
    ID3D11RenderTargetView* getRTV() const;
    bool isValid() const;

  private:
    bool createTexture(D3D11_TEXTURE2D_DESC const& textureDescriptor);

  private:
    bool m_valid = false;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
  };

}

#endif