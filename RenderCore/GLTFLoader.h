#ifndef GLTF_LOADER_H
#define GLTF_LOADER_H

#include "IMeshLoader.h"

struct cgltf_accessor; // Forward declared

namespace rcore {

  class GLTFLoader : public IMeshLoader {
  public:
    Mesh readMesh(std::string const& path) const override;

  private:
    std::vector<float> readAccessor(cgltf_accessor* accessor) const;
  };

}

#endif