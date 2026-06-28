#ifndef VERTEX_BUFFER_BASE_H
#define VERTEX_BUFFER_BASE_H

namespace rcore {

  class VertexBufferBase {
  public:
    virtual ~VertexBufferBase() = default;
    virtual UINT bind() const = 0;
    virtual bool valid() const = 0;
  };

}

#endif