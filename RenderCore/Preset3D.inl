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

  template<std::derived_from<IMeshLoader> TLoader, std::derived_from<Preset3D::StandardVertexType> TBufferType>
  std::shared_ptr<StaticIndexedVertexBuffer<TBufferType>> Preset3D::makeExtendedSIVBuffer(std::string const& meshFile) {
    TLoader loader{ };
    Mesh mesh = loader.readMesh(meshFile);
    return makeExtendedSIVBuffer<TBufferType>(mesh);
  }

  template<std::derived_from<Preset3D::StandardVertexType> TBufferType>
  std::shared_ptr<StaticIndexedVertexBuffer<TBufferType>> Preset3D::makeExtendedSIVBuffer(Mesh const& mesh) {
    std::vector<TBufferType> verts;

    for (size_t i = 0; i < mesh.vertices.size(); i++) {
      TBufferType vert{};

      vert.position = mesh.vertices.at(i);
      vert.normal = mesh.normals.at(i);
      vert.uv = mesh.uvs.at(i);

      if (mesh.hasTangents) {
        vert.tangent = mesh.tangents.at(i);
        vert.binormal = mesh.binormals.at(i);
      } else {
        vert.tangent = { 1.0f, 0.0f, 0.0f };
        vert.binormal = { 0.0f, 0.0f, 1.0f };
      }

      verts.push_back(vert);
    }

    return std::make_shared<StaticIndexedVertexBuffer<TBufferType>>(verts, mesh.indices);
  }

}

#endif