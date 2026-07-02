#ifndef MESH_LOADER_BASE_H
#define MESH_LOADER_BASE_H

#include "pch.h"
#include "Mesh.h"

namespace rcore {

  class MeshLoaderBase {
  public:
    virtual ~MeshLoaderBase() = default;
    virtual Mesh readMesh(std::string const& path) const = 0;
  };

}

#endif