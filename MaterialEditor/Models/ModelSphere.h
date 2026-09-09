#ifndef MODEL_SPHERE_H
#define MODEL_SPHERE_H

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
#include "NBCS/NBCSTextureGenerator.h"

enum TextureType : int {
  albedo,
  normal,
  displacement,
  roughness,
  metallic,
  ambientOcclusion,
  count
};

class ModelSphere {
private:
  struct __declspec(align(16)) MaterialProperties {
    DirectX::XMFLOAT2 uvScale;
    float globalIllumination;
    float displacementStrength;
    int displacementMethod;
    float nbcsStepSizeFactor;
    DirectX::XMFLOAT2 minMaxPOMLayers;
  };

  struct __declspec(align(16)) CameraBufferData {
    DirectX::XMFLOAT4 camPosition;
  };

public:
  ModelSphere(std::weak_ptr<rcore::Window> window, std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);
  void setProperties();
  void onCamChange(DirectX::XMFLOAT3 pos) const;

  void updateTexture(std::string const& path, TextureType type);

private:
  void createSIVBuffer();
  void createTextures(std::weak_ptr<rcore::Window> window);
  void createMaterial(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);
  void createCam();
  void createModel();
  void bindTextures();

public:
  float uScale = 4.0f;
  float vScale = 4.0f;
  float giStrength = 0.8f;
  float displacement = 0.15f;
  int displacementMethod = 2; // NBCS
  float nbcsStepSizeFactor = 2.5f; // 2.5 or less for plane, 5.0 for sphere
  float minPOMLayers = 100;
  float maxPOMLayers = 250;

private:
  MaterialProperties m_properties;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::array<std::shared_ptr<rcore::Texture>, static_cast<size_t>(TextureType::count)> m_textures;
  std::shared_ptr<rcore::Sampler> m_pointSampler, m_linearSampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::CBuffer<CameraBufferData>> m_camBuffer;
  std::unique_ptr<rcore::Model> m_sphere;
  std::pair<std::shared_ptr<rcore::RenderTarget>, std::shared_ptr<rcore::RenderTarget>> m_nbcsTextures;
};

#endif