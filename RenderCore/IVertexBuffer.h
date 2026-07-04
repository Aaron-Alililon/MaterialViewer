#ifndef I_VERTEX_BUFFER_H
#define I_VERTEX_BUFFER_H

#include "pch.h"

namespace rcore {

  class IVertexBuffer {
  public:
    virtual ~IVertexBuffer() = default;
    virtual UINT bind() const = 0;
    virtual bool valid() const = 0;
  };

}

#endif