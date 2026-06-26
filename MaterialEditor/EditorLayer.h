#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "Shader.h"
#include "StaticIndexedVertexBuffer.h"
#include "MatrixBuffer.h"
#include "MeshLoader.h"
#include "Transform.h"

class EditorLayer : public rcore::Layer {

  struct __declspec(align(16)) VertexType {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT3 normal;
  };

public:
  EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc);

public:
  virtual void update(rcore::FrameState const& frame) override;
  virtual void render(rcore::FrameState const& frame) override;

private:
  rcore::D3DContextDesc m_ctxDesc;
  rcore::Shader m_shader;
  rcore::StaticIndexedVertexBuffer<VertexType> m_SIVBuffer;
  rcore::MatrixBuffer m_matrixBuffer;
};

#endif