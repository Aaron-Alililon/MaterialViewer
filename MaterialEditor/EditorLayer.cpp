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

  inputDesc[1].SemanticName = "COLOR";
  inputDesc[1].SemanticIndex = 0;
  inputDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  inputDesc[1].InputSlot = 0;
  inputDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  inputDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  inputDesc[1].InstanceDataStepRate = 0;

  m_shader = { L"VertexShader.hlsl", L"PixelShader.hlsl", inputDesc };

  createMatrixBuffer();
  createVertexBuffer();
  createIndexBuffer();
}

void EditorLayer::update(rcore::FrameState const& frame) {
  
}

void EditorLayer::render(rcore::FrameState const& frame) {
  m_shader.activate();

  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  setMatrixBuffer();

  UINT stride = sizeof(VertexType);
  UINT offset = 0;
  UINT indexCount = 3;

  rcore::D3D11Device::get().rawContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
  rcore::D3D11Device::get().rawContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
  rcore::D3D11Device::get().rawContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  rcore::D3D11Device::get().rawContext()->DrawIndexed(indexCount, 0, 0);
}

void EditorLayer::setMatrixBuffer() {
  HRESULT result;

  DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
  DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
  DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(3.141592654f / 4.0f, 1, 0.3f, 1000.0f);

  worldMatrix = XMMatrixTranspose(worldMatrix);
  viewMatrix = XMMatrixTranspose(viewMatrix);
  projectionMatrix = XMMatrixTranspose(projectionMatrix);

  D3D11_MAPPED_SUBRESOURCE mappedResource;
  result = rcore::D3D11Device::get().rawContext()->Map(m_matrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result)) {
    return;
  }

  MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  rcore::D3D11Device::get().rawContext()->Unmap(m_matrixBuffer.Get(), 0);

  UINT bufferNumber = 0;
  rcore::D3D11Device::get().rawContext()->VSSetConstantBuffers(bufferNumber, 1, m_matrixBuffer.GetAddressOf());
}

void EditorLayer::createMatrixBuffer() {
  HRESULT result;

  D3D11_BUFFER_DESC matrixBufferDesc{};
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  result = rcore::D3D11Device::get().raw()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
  if (FAILED(result)) {
    return;
  }
}

void EditorLayer::createVertexBuffer() {
  HRESULT result;

  D3D11_BUFFER_DESC vertexBufferDesc{};
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof(VertexType) * 3;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA vertexData{};
  VertexType vertices[] = {
    {{ 0.0f,  0.5f, 5.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{ 0.5f, -0.5f, 5.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{-0.5f, -0.5f, 5.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}
  };
  vertexData.pSysMem = vertices;
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;

  result = rcore::D3D11Device::get().raw()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
  if (FAILED(result)) {
    return;
  }
}

void EditorLayer::createIndexBuffer() {
  HRESULT result;

  D3D11_BUFFER_DESC indexBufferDesc{};
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof(unsigned long) * 3;
  indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA indexData{};
  unsigned int indices[] = { 0, 1, 2 };
  indexData.pSysMem = indices;
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;

  result = rcore::D3D11Device::get().raw()->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
  if (FAILED(result)) {
    return;
  }
}