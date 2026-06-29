#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "Material.h"
#include "StaticIndexedVertexBuffer.h"
#include "MatrixBuffer.h"
#include "MeshLoader.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer3D.h"

class EditorLayer : public rcore::Layer {

  struct __declspec(align(16)) MaterialProperties {
    DirectX::XMFLOAT4 albedo;
    DirectX::XMFLOAT4 sunDirection;
    float globalIllumination;
  };

public:
  EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc);

public:
  void update(rcore::FrameState const& frame) override;
  void render(rcore::FrameState const& frame) override;

private:
  rcore::D3DContextDesc m_ctxDesc;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<rcore::Renderer3D::StandardVertexType>> m_SIVBuffer;
  rcore::Camera m_cam;
  std::unique_ptr<rcore::Model> m_model;
  std::shared_ptr<rcore::MatrixBuffer> m_matrixBuffer;
};

#endif