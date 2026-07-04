#ifndef I_TEXTURE_LOADER_H
#define I_TEXTURE_LOADER_H

#include "pch.h"

namespace rcore {

  class ITextureLoader {
  public:
    virtual ~ITextureLoader() = default;
    virtual unsigned char* readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize) = 0;
  };

}

#endif