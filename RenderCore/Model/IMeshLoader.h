#ifndef I_MESH_LOADER_H
#define I_MESH_LOADER_H

#include "Core/pch.h"
#include "Model/Mesh.h"

namespace rcore {

  class IMeshLoader {
  public:
    virtual ~IMeshLoader() = default;
    virtual Mesh readMesh(std::string const& path) const = 0;
  };

}

#endif