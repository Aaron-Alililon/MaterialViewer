#include "Core/pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Lib/stb_image.h"
#include "Resource/PNGLoader.h"

namespace rcore {

  unsigned char* PNGLoader::readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize) {
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);

    if (!data) {
      RCORE_LOG(ERR, "Failed to load texture: " + path);
      return nullptr;
    }

    if (outImageSize) *outImageSize = std::make_pair(static_cast<UINT>(width), static_cast<UINT>(height));

    return data;
  }

}