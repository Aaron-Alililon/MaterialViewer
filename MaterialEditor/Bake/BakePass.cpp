#include "Bake/BakePass.h"

BakePass::BakePass(std::weak_ptr<rcore::Window> const& window) :
  m_window{ window },
  m_depthStencilState{ rcore::Preset3D::makeDisabledDepthStencilDescription() },
  m_rasterizerState{ rcore::Preset3D::makeNoCullingRasterDescription() }
{}

void BakePass::bake() {
  if (m_baked) {
    RCORE_LOG(rcore::WARN, "Tried baking one bakepass multiple times - Use rebake() if this is intentional");
    return;
  }

  rebake();

  m_baked = true;
}

void BakePass::rebake() {
  bindBakeState();

  if (
    !setupRenderTargets() ||
    !bindSourceData() ||
    !draw()
  ) {
    RCORE_LOG(rcore::ERR, "An error occured during baking");
  }

  restoreContext();
}

void BakePass::bindBakeState() {
  m_depthStencilState.bind();
  m_rasterizerState.bind();
}

void BakePass::restoreContext() {
  auto lockedWindow = m_window.lock();
  if (!lockedWindow) {
    RCORE_LOG(rcore::ERR, "Tried accessing invalid window pointer while baking");
    return;
  }

  lockedWindow->bindContextStates();
  lockedWindow->activateContext();
}