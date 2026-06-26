#ifndef MESH_H
#define MESH_H

#include "pch.h"

namespace rcore {

  struct Mesh {
    std::vector<DirectX::XMFLOAT4> vertices;
    std::vector<DirectX::XMFLOAT2> uvs;
    std::vector<DirectX::XMFLOAT3> normals;
    UINT vertexCount;
    UINT indexCount;
  };

}

#endif