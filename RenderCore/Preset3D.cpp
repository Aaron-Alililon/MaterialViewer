#include "pch.h"
#include "Preset3D.h"

namespace rcore {

  WindowDesc Preset3D::makeStandardWindowDescription(LPCWSTR title, UINT width, UINT height) {
    WindowDesc windDesc;

    windDesc.name(title);
    windDesc.width(width);
    windDesc.height(height);

    return windDesc;
  }

  D3D11_TEXTURE2D_DESC Preset3D::makeStandardDepthBufferDescription(UINT width, UINT height) {
    D3D11_TEXTURE2D_DESC depthBufferDesc{};

    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 4; // 1 if no MSAA
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    return depthBufferDesc;
  }

  D3D11_DEPTH_STENCIL_DESC Preset3D::makeStandardDepthStencilDescription() {
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    return depthStencilDesc;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC Preset3D::makeStandardDepthStencilViewDescription() {
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; // D3D11_DSV_DIMENSION_TEXTURE2D if no MSAA
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    return depthStencilViewDesc;
  }

  D3D11_RASTERIZER_DESC Preset3D::makeStandardRasterDescription() {
    D3D11_RASTERIZER_DESC rasterDesc{};

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = true; // false if no MSAA
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    return rasterDesc;
  }

  D3DContextDesc Preset3D::makeStandardContextDescription(UINT width, UINT height) {
    D3DContextDesc ctxDesc;

    ctxDesc.depthBufferDesc(makeStandardDepthBufferDescription(width, height));
    ctxDesc.depthStencilDesc(makeStandardDepthStencilDescription());
    ctxDesc.depthStencilViewDesc(makeStandardDepthStencilViewDescription());
    ctxDesc.rasterDesc(makeStandardRasterDescription());
    ctxDesc.targetFps(60);

    return ctxDesc;
  }


  std::vector<D3D11_INPUT_ELEMENT_DESC> Preset3D::makeStandardInputDescription() {
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

  std::shared_ptr<StaticIndexedVertexBuffer<Preset3D::StandardVertexType>> Preset3D::makeStandardSIVBuffer(std::string const& meshFile) {
    Mesh mesh = MeshLoader::load(meshFile);
    return makeStandardSIVBuffer(mesh);
  }

  std::shared_ptr<StaticIndexedVertexBuffer<Preset3D::StandardVertexType>> Preset3D::makeStandardSIVBuffer(Mesh const& mesh) {
    std::vector<Preset3D::StandardVertexType> verts;
    std::vector<UINT> indices;

    for (size_t i = 0; i < mesh.vertices.size(); i++) {
      verts.push_back({
        mesh.vertices.at(i),
        mesh.normals.at(i),
        mesh.uvs.at(i)
      });

      indices.push_back(static_cast<UINT>(i));
    }

    return std::make_shared<StaticIndexedVertexBuffer<Preset3D::StandardVertexType>>(verts, indices);
  }

}