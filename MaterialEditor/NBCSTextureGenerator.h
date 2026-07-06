#ifndef NBCS_TEXTURE_GENERATOR_H
#define NBCS_TEXTURE_GENERATOR_H

#include "Window.h"
#include "Preset3D.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"

class NBCSTextureGenerator {
public:
  NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window);

public:
  void bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window);

private:
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
};

#endif