#ifndef MESH_H
#define MESH_H

#include "Core/pch.h"

namespace rcore {

  struct Mesh {
    std::vector<DirectX::XMFLOAT4> vertices;
    std::vector<DirectX::XMFLOAT2> uvs;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<DirectX::XMFLOAT3> tangents;
    std::vector<DirectX::XMFLOAT3> binormals;
    std::vector<UINT> indices;
    bool hasTangents = false;
  };

}

#endif