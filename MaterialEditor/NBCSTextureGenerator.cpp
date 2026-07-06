#include "NBCSTextureGenerator.h"

NBCSTextureGenerator::NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window) :
    m_depthStencilState{ rcore::Preset3D::makeDisabledDepthStencilDescription() },
    m_rasterizerState{ rcore::Preset3D::makeNoCullingRasterDescription() }
{
  bakeTangentBinormalTextures(window);
}

void NBCSTextureGenerator::bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window) {
  m_depthStencilState.bind();
  m_rasterizerState.bind();

  // TODO

  auto lockedWindow = window.lock();
  if (!lockedWindow) { 
    RCORE_LOG(rcore::ERR, "Encountered invalid window pointer while generating tangent and binormal textures for NBCS");
    return;
  }
  lockedWindow->bindContextStates();
}