#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "Shader.h"
#include "StaticIndexedVertexBuffer.h"
#include "MeshLoader.h"

class EditorLayer : public rcore::Layer {

  struct __declspec(align(16)) VertexType {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT3 normal;
  };

  struct __declspec(align(16)) MatrixBufferType {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

public:
  EditorLayer(std::weak_ptr<rcore::Window> window);

public:
  virtual void update(rcore::FrameState const& frame) override;
  virtual void render(rcore::FrameState const& frame) override;

private:
  void setMatrixBuffer();
  void createMatrixBuffer();

private:
  rcore::Shader m_shader;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
  rcore::StaticIndexedVertexBuffer<VertexType> m_SIVBuffer;
};

#endif