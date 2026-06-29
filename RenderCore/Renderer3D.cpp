#include "pch.h"
#include "Renderer3D.h"

namespace rcore {

  std::shared_ptr<StaticIndexedVertexBuffer<Renderer3D::StandardVertexType>> Renderer3D::makeStandardSIVBuffer(std::string meshFile) {
    Mesh mesh = MeshLoader::load(meshFile);
    return makeStandardSIVBuffer(mesh);
  }

  std::shared_ptr<StaticIndexedVertexBuffer<Renderer3D::StandardVertexType>> Renderer3D::makeStandardSIVBuffer(Mesh const& mesh) {
    std::vector<Renderer3D::StandardVertexType> verts;
    std::vector<UINT> indices;

    for (size_t i = 0; i < mesh.vertices.size(); i++) {
      verts.push_back({
        mesh.vertices.at(i),
        mesh.normals.at(i),
        mesh.uvs.at(i)
      });

      indices.push_back(static_cast<UINT>(i));
    }

    return std::make_shared<StaticIndexedVertexBuffer<Renderer3D::StandardVertexType>>(verts, indices);
  }

  std::vector<D3D11_INPUT_ELEMENT_DESC> Renderer3D::makeStandardInputDescription() {
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc(3);

    inputDesc[0].SemanticName = "POSITION";
    inputDesc[0].SemanticIndex = 0;
    inputDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputDesc[0].InputSlot = 0;
    inputDesc[0].AlignedByteOffset = 0;
    inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[0].InstanceDataStepRate = 0;

    inputDesc[1].SemanticName = "NORMAL";
    inputDesc[1].SemanticIndex = 0;
    inputDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc[1].InputSlot = 0;
    inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[1].InstanceDataStepRate = 0;

    inputDesc[2].SemanticName = "TEXCOORD";
    inputDesc[2].SemanticIndex = 0;
    inputDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputDesc[2].InputSlot = 0;
    inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[2].InstanceDataStepRate = 0;

    return inputDesc;
  }

}