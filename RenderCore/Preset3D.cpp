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

  D3D11_DEPTH_STENCIL_DESC Preset3D::makeDisabledDepthStencilDescription() {
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = makeStandardDepthStencilDescription();

    depthStencilDesc.DepthEnable = false;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

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

  D3D11_RASTERIZER_DESC Preset3D::makeNoCullingRasterDescription() {
    D3D11_RASTERIZER_DESC rasterDesc = makeStandardRasterDescription();

    rasterDesc.CullMode = D3D11_CULL_NONE;

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
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc(5);

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

    inputDesc[3].SemanticName = "TANGENT";
    inputDesc[3].SemanticIndex = 0;
    inputDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc[3].InputSlot = 0;
    inputDesc[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[3].InstanceDataStepRate = 0;

    inputDesc[4].SemanticName = "BINORMAL";
    inputDesc[4].SemanticIndex = 0;
    inputDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputDesc[4].InputSlot = 0;
    inputDesc[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputDesc[4].InstanceDataStepRate = 0;

    return inputDesc;
  }

  std::shared_ptr<StaticIndexedVertexBuffer<Preset3D::StandardVertexType>> Preset3D::makeStandardSIVBuffer(Mesh const& mesh, bool keepCPUData) {
    std::vector<StandardVertexType> verts;

    for (size_t i = 0; i < mesh.vertices.size(); i++) {
      StandardVertexType vert{};
      
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

    return std::make_shared<StaticIndexedVertexBuffer<StandardVertexType>>(verts, mesh.indices, keepCPUData);
  }

  D3D11_TEXTURE2D_DESC Preset3D::makeStandardTextureDescription() {
    D3D11_TEXTURE2D_DESC desc{};

    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    return desc;
  }

  D3D11_TEXTURE2D_DESC Preset3D::makeRenderTargetTextureDescription(UINT width, UINT height) {
    D3D11_TEXTURE2D_DESC desc = makeStandardTextureDescription();

    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;

    return desc;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC Preset3D::makeStandardTextureShaderResourceViewDescription() {
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    return srvDesc;
  }

  std::pair<D3D11_TEXTURE2D_DESC, D3D11_SHADER_RESOURCE_VIEW_DESC> Preset3D::makeStandardTextureDescriptionPair() {
    return std::make_pair(
      makeStandardTextureDescription(),
      makeStandardTextureShaderResourceViewDescription()
    );
  }

  D3D11_SAMPLER_DESC Preset3D::makeStandardPointSamplerDescription() {
    D3D11_SAMPLER_DESC samplerDesc{};

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return samplerDesc;
  }

  D3D11_SAMPLER_DESC Preset3D::makeStandardLinearSamplerDescription() {
    D3D11_SAMPLER_DESC samplerDesc{};

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return samplerDesc;
  }

}