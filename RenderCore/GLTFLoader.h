#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include "MeshLoaderBase.h"

struct cgltf_accessor; // Forward declared

namespace rcore {

  class GLTFLoader : public MeshLoaderBase {
  public:
    Mesh readMesh(std::string const& path) const override;

  private:
    std::vector<float> readAccessor(cgltf_accessor* accessor) const;
  };

}

#endif