#ifndef SKYBOX_H
#define SKYBOX_H

#include "Core/FrameState.h"
#include "D3D11/Buffer/StaticIndexedVertexBuffer.h"
#include "Core/Preset3D.h"
#include "Resource/Texture.h"
#include "Resource/Sampler.h"
#include "Render/Material.h"
#include "Model/Model.h"
#include "Resource/PNGLoader.h"
#include "D3D11/Buffer/LightBuffer.h"

class Skybox {
private:
  struct __declspec(align(16)) MaterialProperties {};

public:
  Skybox(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);
  void setType();

private:
  void createSIVBuffer();
  void createSampler();
  void createTextures();
  void createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);
  void createModel();

public:
  int selectedSkybox = 0;

private:
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::shared_ptr<rcore::Sampler> m_sampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::Model> m_box;
  std::vector<rcore::Texture> m_textures;
};

#endif