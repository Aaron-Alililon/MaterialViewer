#include "EditorLayer.h"

EditorLayer::EditorLayer(std::weak_ptr<rcore::Window> window) : Layer(window) {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc(2);

  inputDesc[0].SemanticName = "POSITION";
  inputDesc[0].SemanticIndex = 0;
  inputDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  inputDesc[0].InputSlot = 0;
  inputDesc[0].AlignedByteOffset = 0;
  inputDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  inputDesc[0].InstanceDataStepRate = 0;

  inputDesc[1].SemanticName = "NORMAL";
  inputDesc[1].SemanticIndex = 0;
  inputDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  inputDesc[1].InputSlot = 0;
  inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  inputDesc[1].InstanceDataStepRate = 0;

  m_shader = { L"VertexShader.hlsl", L"PixelShader.hlsl", inputDesc };

  rcore::Mesh sphere = rcore::MeshLoader::load("models/sphere.obj");
  std::vector<VertexType> verts;
  std::vector<UINT> indices;

  for (size_t i = 0; i < sphere.vertices.size(); i++) {
    verts.push_back({
      sphere.vertices.at(i),
      sphere.normals.at(i)
    });

    indices.push_back(static_cast<UINT>(i));
  }

  m_SIVBuffer.createBuffers(verts, indices);
}

void EditorLayer::update(rcore::FrameState const& frame) {
  
}

void EditorLayer::render(rcore::FrameState const& frame) {
  m_shader.activate();

  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  rcore::Transform transform{
    { 0, 0, 0 },
    { 0, frame.frameCount * 0.01f, 0 },
    { 2, 2, 1 }
  };

  m_matrixBuffer.setMatrices({
    transform.getWorldMatrix(),
    DirectX::XMMatrixLookAtLH(
      DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
      DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
      DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    DirectX::XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, 1, 0.3f, 1000.0f)
  });

  UINT indexCount = m_SIVBuffer.bind();
  rcore::D3D11Device::get().rawContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  rcore::D3D11Device::get().rawContext()->DrawIndexed(indexCount, 0, 0);
}