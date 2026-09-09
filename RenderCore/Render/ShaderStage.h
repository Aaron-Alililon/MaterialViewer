#ifndef SHADER_STAGE_H
#define SHADER_STAGE_H

#include "Core/pch.h"

namespace rcore {

  enum ShaderStage : uint8_t {
    Vertex = 1 << 0,
    Pixel = 1 << 1,
  };

}

#endif