#ifndef SKYBOX_H
#define SKYBOX_H

#include "FrameState.h"
#include "StaticIndexedVertexBuffer.h"
#include "Preset3D.h"
#include "Texture.h"
#include "Sampler.h"
#include "Material.h"
#include "Model.h"
#include "PNGLoader.h"

class Skybox {
private:
  struct __declspec(align(16)) MaterialProperties {};

public:
  Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);

private:
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::shared_ptr<rcore::Texture> m_textureAlbedo;
  std::shared_ptr<rcore::Sampler> m_sampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::Model> m_box;
};

#endif