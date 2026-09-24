#ifndef DISPLAY_MODEL_H
#define DISPLAY_MODEL_H

#include "Render/Material.h"
#include "Model/Model.h"
#include "D3D11/Buffer/StaticIndexedVertexBuffer.h"
#include "Core/FrameState.h"
#include "D3D11/Buffer/MatrixBuffer.h"
#include "Core/Preset3D.h"
#include "Resource/Texture.h"
#include "Resource/Sampler.h"
#include "Model/GLTFLoader.h"
#include "Model/ObjLoader.h"
#include "Resource/PNGLoader.h"
#include "Bake/NBCS/NBCSTextureGenerator.h"
#include "Models/Skybox.h"

enum TextureType : int {
  albedo,
  normal,
  displacement,
  roughness,
  metallic,
  ambientOcclusion,
  count
};

class DisplayModel {
private:
  struct __declspec(align(16)) MaterialProperties {
    DirectX::XMFLOAT2 uvScale;
    float globalIllumination;
    int tonemapMethod;
    float exposure;
    float displacementStrength;
    int displacementMethod;
    float nbcsStepSizeFactor;
    DirectX::XMFLOAT2 minMaxPOMLayers;
  };

  struct __declspec(align(16)) CameraBufferData {
    DirectX::XMFLOAT4 camPosition;
  };

  struct ModelData {
    std::unique_ptr<rcore::Model> model;
    std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> SIVBuffer;
    std::pair<std::shared_ptr<rcore::RenderTarget>, std::shared_ptr<rcore::RenderTarget>> nbcsTextures;
  };

public:
  DisplayModel(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);
  void setModel();
  void setProperties();
  void onCamChange(DirectX::XMFLOAT3 pos) const;

  void updateTexture(std::string const& path, TextureType type);
  void updateSkyboxData(Skybox::SkyboxData data);

private:
  void createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);
  void createTextures();
  void createSIVBuffer(ModelData& data, std::string const& path);
  void createNbcsTextures(ModelData& data, std::weak_ptr<rcore::Window> window);
  void createModels(std::weak_ptr<rcore::Window> window);
  void createCam();
  void bindTextures();

public:
  int selectedModel = 0; // Sphere
  float uScale = 4.0f;
  float vScale = 4.0f;
  float giStrength = 1.0f;
  int tonemapMethod; // Set by ViewLayer
  float exposure; // Set by ViewLayer
  float displacement = 0.15f;
  int displacementMethod = 2; // NBCS
  float nbcsStepSizeFactor = 5.0f; // 2.5 or less for plane, 5.0 for sphere
  float minPOMLayers = 100;
  float maxPOMLayers = 250;

private:
  MaterialProperties m_properties;
  std::vector<ModelData> m_models;
  std::array<std::shared_ptr<rcore::Texture>, static_cast<size_t>(TextureType::count)> m_textures;
  std::shared_ptr<rcore::Sampler> m_pointSampler, m_linearSampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::CBuffer<CameraBufferData>> m_camBuffer;
  Skybox::SkyboxData m_skyboxData;
};

#endif