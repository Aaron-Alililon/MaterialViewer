#ifndef LAYER_H
#define LAYER_H

#include "FrameState.h"

namespace rcore {

  class Layer {
  public:
    virtual void setup() {}
    virtual void update(FrameState const& frame) {}
    virtual void render(FrameState const& frame) {}
  };

}

#endif