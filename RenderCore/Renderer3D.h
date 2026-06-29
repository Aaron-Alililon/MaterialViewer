#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include "StaticIndexedVertexBuffer.h"
#include "MeshLoader.h"

namespace rcore {

  class Renderer3D {
  public:
    struct __declspec(align(16)) StandardVertexType {
      DirectX::XMFLOAT4 position;
      DirectX::XMFLOAT3 normal;
      DirectX::XMFLOAT2 uv;
    };

  public:
    Renderer3D() = delete;

  public:
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(std::string meshFile);
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(Mesh const& mesh);

    static std::vector<D3D11_INPUT_ELEMENT_DESC> makeStandardInputDescription();
  };

}

#endif