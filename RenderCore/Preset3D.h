#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include "WindowDesc.h"
#include "D3D11Context.h"
#include "StaticIndexedVertexBuffer.h"
#include "MeshLoader.h"

namespace rcore {

  class Preset3D {
  public:
    struct __declspec(align(16)) StandardVertexType {
      DirectX::XMFLOAT4 position;
      DirectX::XMFLOAT3 normal;
      DirectX::XMFLOAT2 uv;
    };

  public:
    Preset3D() = delete;

  public:
    static WindowDesc makeStandardWindowDescription(LPCWSTR title = L"DirectX11 Window", UINT width = 400, UINT height = 400);

    static D3D11_TEXTURE2D_DESC makeStandardDepthBufferDescription(UINT width, UINT height);
    static D3D11_DEPTH_STENCIL_DESC makeStandardDepthStencilDescription();
    static D3D11_DEPTH_STENCIL_VIEW_DESC makeStandardDepthStencilViewDescription();
    static D3D11_RASTERIZER_DESC makeStandardRasterDescription();
    static D3DContextDesc makeStandardContextDescription(UINT width, UINT height);

    static std::vector<D3D11_INPUT_ELEMENT_DESC> makeStandardInputDescription();
    
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(std::string const& meshFile);
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(Mesh const& mesh);
  };

}

#endif