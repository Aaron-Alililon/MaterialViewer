#ifndef D3D11_CONTEXT_DESC_H
#define D3D11_CONTEXT_DESC_H

namespace rcore {

  class D3DContextDesc {
  public:
    void depthBufferDesc(D3D11_TEXTURE2D_DESC depthBufferDesc);
    D3D11_TEXTURE2D_DESC depthBufferDesc() const;

    void depthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc);
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc() const;

    void depthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc);
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc() const;

    void rasterDesc(D3D11_RASTERIZER_DESC rasterDesc);
    D3D11_RASTERIZER_DESC rasterDesc() const;

    void targetFps(unsigned int targetFps);
    unsigned int targetFps() const;

  private:
    D3D11_TEXTURE2D_DESC m_depthBufferDescriptor = {};
    D3D11_DEPTH_STENCIL_DESC m_depthStencilDescriptor = {};
    D3D11_DEPTH_STENCIL_VIEW_DESC m_depthStencilViewDescriptor = {};
    D3D11_RASTERIZER_DESC m_rasterDescriptor = {};
    unsigned int m_targetFps = 60;
  };

}

#endif