#ifndef MODEL_SPHERE_H
#define MODEL_SPHERE_H

#include "Material.h"
#include "Model.h"
#include "StaticIndexedVertexBuffer.h"
#include "FrameState.h"
#include "MatrixBuffer.h"
#include "Preset3D.h"
#include "Texture.h"
#include "Sampler.h"
#include "GLTFLoader.h"
#include "ObjLoader.h"
#include "PNGLoader.h"

class ModelSphere {
private:
  struct __declspec(align(16)) MaterialProperties {
    DirectX::XMFLOAT4 sunDirection;
    DirectX::XMFLOAT2 uvScale;
    float globalIllumination;
    float displacementStrength;
  };

public:
  ModelSphere(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);
  void onValueChange();

public:
  float uScale = 8.0f;
  float vScale = 4.0f;
  float displacement = 0.15f;
  float giStrength = 0.6f;

private:
  MaterialProperties m_properties;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::shared_ptr<rcore::Texture> m_textureAlbedo, m_textureNormal, m_textureDisplacement, m_textureRoughness;
  std::shared_ptr<rcore::Sampler> m_sampler;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::unique_ptr<rcore::Model> m_sphere;
};

#endif