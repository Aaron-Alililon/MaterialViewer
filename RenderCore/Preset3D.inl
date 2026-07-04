#ifndef PRESET_3D_INL
#define PRESET_3D_INL

#include "Preset3D.h"

namespace rcore {

  template<std::derived_from<IMeshLoader> TLoader>
  std::shared_ptr<StaticIndexedVertexBuffer<Preset3D::StandardVertexType>> Preset3D::makeStandardSIVBuffer(std::string const& meshFile) {
    TLoader loader{ };
    Mesh mesh = loader.readMesh(meshFile);
    return makeStandardSIVBuffer(mesh);
  }

}

#endif