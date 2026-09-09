#include "Core/pch.h"
#define CGLTF_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4996)
#include "Lib/cgltf.h"
#pragma warning(pop)
#include "Model/GLTFLoader.h"

namespace rcore {

  Mesh GLTFLoader::readMesh(std::string const& path) const {
    Mesh mesh;

    cgltf_options options = {};
    cgltf_data* data = nullptr;

    if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success) {
      RCORE_LOG(ERR, "Failed to parse GLTF file: " + path);
      return {};
    }

    if (cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success) {
      RCORE_LOG(ERR, "Failed to load GLTF buffers: " + path);
      cgltf_free(data);
      return {};
    }

    cgltf_mesh& gltfMesh = data->meshes[0];
    cgltf_primitive& prim = gltfMesh.primitives[0];

    std::vector<float> positions, normals, tangents, texcoords;

    for (size_t i = 0; i < prim.attributes_count; i++) {
      cgltf_attribute& attr = prim.attributes[i];
      if (attr.type == cgltf_attribute_type_position) positions = readAccessor(attr.data);
      if (attr.type == cgltf_attribute_type_normal)   normals = readAccessor(attr.data);
      if (attr.type == cgltf_attribute_type_tangent)  tangents = readAccessor(attr.data);
      if (attr.type == cgltf_attribute_type_texcoord) texcoords = readAccessor(attr.data);
    }

    std::vector<UINT> indices(prim.indices->count);
    for (size_t i = 0; i < prim.indices->count; i++)
      indices[i] = static_cast<UINT>(cgltf_accessor_read_index(prim.indices, i));

    size_t vertexCount = positions.size() / 3;

    mesh.vertices.resize(vertexCount);
    mesh.normals.resize(vertexCount);
    mesh.uvs.resize(vertexCount);
    mesh.tangents.resize(vertexCount);
    mesh.binormals.resize(vertexCount);

    for (size_t i = 0; i < vertexCount; i++) {
      mesh.vertices[i] = { positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2], 1.0f };
      mesh.normals[i]  = { normals[i * 3],   normals[i * 3 + 1],   normals[i * 3 + 2] };
      mesh.uvs[i]      = { texcoords[i * 2], texcoords[i * 2 + 1] };

      if (!tangents.empty()) {
        DirectX::XMFLOAT3 t = { tangents[i * 4], tangents[i * 4 + 1], tangents[i * 4 + 2] };
        float w = tangents[i * 4 + 3];
        DirectX::XMFLOAT3 n = mesh.normals[i];

        mesh.tangents[i] = t;
        mesh.binormals[i] = {
            (n.y * t.z - n.z * t.y) * w * -1,
            (n.z * t.x - n.x * t.z) * w * -1,
            (n.x * t.y - n.y * t.x) * w * -1
        };
      } else {
        mesh.tangents[i] = { 1.0f, 0.0f, 0.0f };
        mesh.binormals[i] = { 0.0f, 0.0f, 1.0f };
      }
    }

    mesh.indices = std::move(indices);
    mesh.hasTangents = !tangents.empty();

    cgltf_free(data);
    return mesh;
  }

  std::vector<float> GLTFLoader::readAccessor(cgltf_accessor* accessor) const {
    std::vector<float> out(accessor->count * cgltf_num_components(accessor->type));
    cgltf_accessor_unpack_floats(accessor, out.data(), out.size());
    return out;
  }

}