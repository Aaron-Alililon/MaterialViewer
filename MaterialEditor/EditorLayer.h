#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "Shader.h"

class EditorLayer : public rcore::Layer {

  struct __declspec(align(16)) VertexType {
    DirectX::XMFLOAT4 position;
    DirectX::XMFLOAT4 color;
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
  void createVertexBuffer();
  void createIndexBuffer();

private:
  rcore::Shader m_shader;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
};

#endif