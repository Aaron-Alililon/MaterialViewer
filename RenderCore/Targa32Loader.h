#ifndef TARGA_32_LOADER_H
#define TARGA_32_LOADER_H

#include "ITextureLoader.h"

namespace rcore {
  
  class Targa32Loader : public ITextureLoader {
  private:
    struct TargaHeader {
      unsigned char data1[12];
      unsigned short width;
      unsigned short height;
      unsigned char bpp;
      unsigned char data2;
    };

  public:
    unsigned char* readTexture(std::string const& path, std::pair<UINT, UINT>* outImageSize);
  };

}

#endif