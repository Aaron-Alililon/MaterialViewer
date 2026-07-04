#ifndef TEXTURE_INL
#define TEXTURE_INL

#include "Texture.h"

namespace rcore {

  template<std::derived_from<ITextureLoader> TLoader>
  Texture::Texture(LoaderTag<TLoader>, std::string const& path, D3D11_TEXTURE2D_DESC const& textureDescriptor, D3D11_SHADER_RESOURCE_VIEW_DESC const& resourceViewDescriptor, UINT width, UINT height) {
		if (loadTexture<TLoader>(path, textureDescriptor, resourceViewDescriptor, width, height)) {
			m_valid = true;
		}
  }

	template<std::derived_from<ITextureLoader> TLoader>
	bool Texture::loadTexture(std::string const& path, D3D11_TEXTURE2D_DESC const& textureDescriptor, D3D11_SHADER_RESOURCE_VIEW_DESC const& resourceViewDescriptor, UINT width, UINT height) {
		HRESULT result;
		D3D11_TEXTURE2D_DESC mutableTextureDescriptor{ textureDescriptor };

		TLoader loader{};
		std::pair<UINT, UINT> imageSize;
		unsigned char* data = loader.readTexture(path, &imageSize);

		mutableTextureDescriptor.Width = (width == 0) ? imageSize.first : width;
		mutableTextureDescriptor.Height = (height == 0) ? imageSize.second : height;

		result = D3D11Device::get().raw()->CreateTexture2D(&mutableTextureDescriptor, nullptr, m_texture.GetAddressOf());
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create texture");
			return false;
		}

		UINT rowPitch = (mutableTextureDescriptor.Width * 4) * static_cast<UINT>(sizeof(unsigned char));
		D3D11Device::get().rawContext()->UpdateSubresource(m_texture.Get(), 0, nullptr, data, rowPitch, 0);

		result = D3D11Device::get().raw()->CreateShaderResourceView(m_texture.Get(), &resourceViewDescriptor, m_textureView.GetAddressOf());
		if (FAILED(result)) {
			RCORE_LOG(ERR, "Failed to create shader resource view");
			return false;
		}

		D3D11Device::get().rawContext()->GenerateMips(m_textureView.Get());

		return true;
	}

}

#endif