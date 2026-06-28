#include "pch.h"
#include "D3DContextDesc.h"

namespace rcore {

  void D3DContextDesc::depthBufferDesc(D3D11_TEXTURE2D_DESC depthBufferDesc) {
    m_depthBufferDescriptor = depthBufferDesc;
  }

  D3D11_TEXTURE2D_DESC D3DContextDesc::depthBufferDesc() const {
    return m_depthBufferDescriptor;
  }

  void D3DContextDesc::depthStencilDesc(D3D11_DEPTH_STENCIL_DESC depthStencilDesc) {
    m_depthStencilDescriptor = depthStencilDesc;
  }

  D3D11_DEPTH_STENCIL_DESC D3DContextDesc::depthStencilDesc() const {
    return m_depthStencilDescriptor;
  }

  void D3DContextDesc::depthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc) {
    m_depthStencilViewDescriptor = depthStencilViewDesc;
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC D3DContextDesc::depthStencilViewDesc() const {
    return m_depthStencilViewDescriptor;
  }

  void D3DContextDesc::rasterDesc(D3D11_RASTERIZER_DESC rasterDesc) {
    m_rasterDescriptor = rasterDesc;
  }

  D3D11_RASTERIZER_DESC D3DContextDesc::rasterDesc() const {
    return m_rasterDescriptor;
  }

  void D3DContextDesc::targetFps(unsigned int targetFps) {
    m_targetFps = targetFps;
  }

  unsigned int D3DContextDesc::targetFps() const {
    return m_targetFps;
  }

}