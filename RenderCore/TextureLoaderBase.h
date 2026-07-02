#ifndef TEXTURE_LOADER_BASE_H
#define TEXTURE_LOADER_BASE_H

#include "pch.h"

namespace rcore {

  class TextureLoaderBase {
  public:
    virtual ~TextureLoaderBase() = default;
    virtual unsigned char* readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize) = 0;
  };

}

#endif