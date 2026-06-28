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

class EditorLayer : public rcore::Layer {

  struct __declspec(align(16)) VertexType {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 uv;
  };

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
  void setSIVBufferData(rcore::Model const& model);

private:
  rcore::D3DContextDesc m_ctxDesc;
  std::shared_ptr<rcore::Material<MaterialProperties>> m_material;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<VertexType>> m_SIVBuffer;
  rcore::Camera m_cam;
  rcore::Model m_model;
  rcore::MatrixBuffer m_matrixBuffer;
};

#endif