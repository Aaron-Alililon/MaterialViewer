#ifndef MODEL_SPHERE_H
#define MODEL_SPHERE_H

#include "Material.h"
#include "Model.h"
#include "StaticIndexedVertexBuffer.h"
#include "FrameState.h"
#include "MatrixBuffer.h"
#include "Preset3D.h"

class ModelSphere {
private:
  struct __declspec(align(16)) MaterialProperties {
    DirectX::XMFLOAT4 albedo;
    DirectX::XMFLOAT4 sunDirection;
    float globalIllumination;
  };

public:
  ModelSphere(std::vector<D3D11_INPUT_ELEMENT_DESC> const& inputDesc);

public:
  void render(rcore::FrameState const& frame, rcore::MatrixBuffer& matrixBuffer);

private:
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_SIVBuffer;
  std::unique_ptr<rcore::Model> m_model;
};

#endif