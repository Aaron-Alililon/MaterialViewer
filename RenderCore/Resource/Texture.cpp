#include "Core/pch.h"
#include "Resource/Texture.h"

namespace rcore {

  ID3D11ShaderResourceView* Texture::getTextureView() const {
    return m_textureView.Get();
  }

  UINT Texture::getWidth() const {
    return m_size.first;
  }

  UINT Texture::getHeight() const {
    return m_size.second;
  }

}