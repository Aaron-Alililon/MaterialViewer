#ifndef BAKE_PASS_H
#define BAKE_PASS_H

#include "Core/Preset3D.h"
#include "Window/Window.h"

class BakePass {
public:
  BakePass(std::weak_ptr<rcore::Window> const& window);
  virtual ~BakePass() = default;

public:
  void bake();
  void rebake();

protected:
  virtual bool setupRenderTargets() = 0;
  virtual bool bindSourceData() = 0;
  virtual bool draw() = 0;

private:
  void bindBakeState();
  void restoreContext();

private:
  bool m_baked = false;
  std::weak_ptr<rcore::Window> m_window;
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
};

#endif