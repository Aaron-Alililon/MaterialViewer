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
#include "Resource/HDRLoader.h"
#include "D3D11/Buffer/LightBuffer.h"
#include "Bake/IBL/EquirectToCubeBaker.h"
#include "Bake/IBL/IrradianceBaker.h"
#include "Bake/IBL/SpecularBaker.h"
#include "Bake/IBL/BRDFLUTBaker.h"

class Skybox {
private:
  struct __declspec(align(16)) MaterialProperties {
    int tonemapMethod;
    float exposure;
  };

public:
  struct SkyboxData {
    std::shared_ptr<rcore::RenderTarget> envCube;
    std::shared_ptr<rcore::RenderTarget> irradianceCube;
    std::shared_ptr<rcore::RenderTarget> specularCube;
    std::shared_ptr<rcore::RenderTarget> brdfLut;
  };

public:
  Skybox(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);
  void setProperties();
  SkyboxData setType();

private:
  void bakeIBL(std::weak_ptr<rcore::Window> window);
  void createSIVBuffer();
  void createSampler();
  void createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);
  void createModel();

public:
  int selectedSkybox = 0; // Sky
  int tonemapMethod; // Set by ViewLayer
  float exposure; // Set by ViewLayer

private:
  MaterialProperties m_properties;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::shared_ptr<rcore::Sampler> m_sampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::Model> m_box;
  std::vector<SkyboxData> m_skyboxes;
};

#endif