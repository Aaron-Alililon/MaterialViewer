#ifndef TEXTURE_H
#define TEXTURE_H

#include "ITextureLoader.h"
#include "D3D11Device.h"

namespace rcore {

  template<std::derived_from<ITextureLoader> T>
  struct LoaderTag{};

  class Texture {
  public:
    template<std::derived_from<ITextureLoader> TLoader>
    Texture(LoaderTag<TLoader>, std::string const& path, D3D11_TEXTURE2D_DESC const& textureDescriptor, D3D11_SHADER_RESOURCE_VIEW_DESC const& resourceViewDescriptor, UINT width = 0, UINT height = 0);

  public:
    ID3D11ShaderResourceView* getTextureView() const;
    UINT getWidth() const;
    UINT getHeight() const;

  private:
    template<std::derived_from<ITextureLoader> TLoader>
    bool loadTexture(std::string const& path, D3D11_TEXTURE2D_DESC const& textureDescriptor, D3D11_SHADER_RESOURCE_VIEW_DESC const& resourceViewDescriptor, UINT width, UINT height);

  private:
    bool m_valid = false;
    std::pair<UINT, UINT> m_size;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
  };

}

#include "Texture.inl"

#endif