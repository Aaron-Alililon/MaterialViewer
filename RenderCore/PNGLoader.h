#ifndef PNG_LOADER_H
#define PNG_LOADER_H

#include "TextureLoaderBase.h"

namespace rcore {

  class PNGLoader : public TextureLoaderBase {
  public:
    unsigned char* readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize);
  };

}

#endif