#ifndef D3D11_CONTEXT_H
#define D3D11_CONTEXT_H

#include "D3DContextDesc.h"

namespace rcore {

  class D3D11Context {
  public:
    D3D11Context(D3DContextDesc const& descriptor);
  };

}

#endif