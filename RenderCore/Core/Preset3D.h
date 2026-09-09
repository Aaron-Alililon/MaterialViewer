#ifndef PRESET_3D_H
#define PRESET_3D_H

#include "Window/WindowDesc.h"
#include "D3D11/D3D11Context.h"
#include "D3D11/Buffer/StaticIndexedVertexBuffer.h"
#include "Model/ObjLoader.h"

namespace rcore {

  class Preset3D {
  public:
    struct StandardVertexType {
      DirectX::XMFLOAT4 position;
      DirectX::XMFLOAT3 normal;
      DirectX::XMFLOAT2 uv;
      DirectX::XMFLOAT3 tangent;
      DirectX::XMFLOAT3 binormal;
    };

  public:
    Preset3D() = delete;

  public:
    static WindowDesc makeStandardWindowDescription(LPCWSTR title = L"DirectX11 Window", UINT width = 400, UINT height = 400);

    static D3D11_TEXTURE2D_DESC makeStandardDepthBufferDescription(UINT width, UINT height);
    static D3D11_DEPTH_STENCIL_DESC makeStandardDepthStencilDescription();
    static D3D11_DEPTH_STENCIL_DESC makeDisabledDepthStencilDescription();
    static D3D11_DEPTH_STENCIL_VIEW_DESC makeStandardDepthStencilViewDescription();
    static D3D11_RASTERIZER_DESC makeStandardRasterDescription();
    static D3D11_RASTERIZER_DESC makeNoCullingRasterDescription();
    static D3DContextDesc makeStandardContextDescription(UINT width, UINT height);

    static std::vector<D3D11_INPUT_ELEMENT_DESC> makeStandardInputDescription();
    
    template<std::derived_from<IMeshLoader> TLoader>
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(std::string const& meshFile, bool keepCPUData = false);
    template<std::derived_from<IMeshLoader> TLoader, std::derived_from<Preset3D::StandardVertexType> TBufferType>
    static std::shared_ptr<StaticIndexedVertexBuffer<TBufferType>> makeExtendedSIVBuffer(std::string const& meshFile, bool keepCPUData = false);
    template<std::derived_from<Preset3D::StandardVertexType> TBufferType>
    static std::shared_ptr<StaticIndexedVertexBuffer<TBufferType>> makeExtendedSIVBuffer(Mesh const& mesh, bool keepCPUData = false);
    static std::shared_ptr<StaticIndexedVertexBuffer<StandardVertexType>> makeStandardSIVBuffer(Mesh const& mesh, bool keepCPUData = false);

    static D3D11_TEXTURE2D_DESC makeStandardTextureDescription();
    static D3D11_TEXTURE2D_DESC makeRenderTargetTextureDescription(UINT width, UINT height);
    static D3D11_SHADER_RESOURCE_VIEW_DESC makeStandardTextureShaderResourceViewDescription();
    static std::pair<D3D11_TEXTURE2D_DESC, D3D11_SHADER_RESOURCE_VIEW_DESC> makeStandardTextureDescriptionPair();

    static D3D11_SAMPLER_DESC makeStandardPointSamplerDescription();
    static D3D11_SAMPLER_DESC makeStandardLinearSamplerDescription();
  };

}

#include "Core/Preset3D.inl"

#endif