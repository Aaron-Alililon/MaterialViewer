#ifndef PNG_LOADER_H
#define PNG_LOADER_H

#include "ITextureLoader.h"

namespace rcore {

  class PNGLoader : public ITextureLoader {
  public:
    unsigned char* readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize);
  };

}

#endif