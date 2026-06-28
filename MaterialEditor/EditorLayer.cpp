#include "EditorLayer.h"

EditorLayer::EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc) : Layer(window), m_ctxDesc{ contextDesc } {
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc(3);

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

  inputDesc[2].SemanticName = "TEXCOORD";
  inputDesc[2].SemanticIndex = 0;
  inputDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
  inputDesc[2].InputSlot = 0;
  inputDesc[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  inputDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  inputDesc[2].InstanceDataStepRate = 0;

  m_cam = { 0, 0, -10 };

  rcore::Shader shader = { L"VertexShader.hlsl", L"PixelShader.hlsl", inputDesc };
  m_material = std::make_shared<rcore::Material<MaterialProperties>>(shader, 1, rcore::Pixel);
  m_material->setProperties({
    { 0.4f, 0.5f, 1.0f, 1.0f },
    { 1.0f, 1.0f, -1.0f, 0.0f },
    0.1f
  });

  rcore::Transform transform{
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 2, 2, 1 }
  };

  m_SIVBuffer = std::make_shared<rcore::StaticIndexedVertexBuffer<VertexType>>();
  
  m_model = { "models/sphere.obj", transform, m_material, m_SIVBuffer };
  setSIVBufferData(m_model);

  m_matrixBuffer = { 0 };
}

void EditorLayer::update(rcore::FrameState const& frame) {
  m_matrixBuffer.setProjectionMatrix(m_cam.getPerspectiveMatrix((float)frame.width / frame.height));
  m_matrixBuffer.setViewMatrix(m_cam.getViewMatrix());
}

void EditorLayer::render(rcore::FrameState const& frame) {
  auto window = m_window.lock();
  if (!window) return;

  float bgCol[] = { 0.1f, 0.15f, 0.2f, 1.0f };
  rcore::D3D11Device::get().rawContext()->ClearRenderTargetView(window->getRenderTargetView(), bgCol);
  rcore::D3D11Device::get().rawContext()->ClearDepthStencilView(window->getDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);

  m_model.setRotation(0, frame.frameCount * 0.01f, 0);
  m_model.drawIndexed(m_matrixBuffer);
}

// TODO find better way to set buffer data encapsulated in model or similar
void EditorLayer::setSIVBufferData(rcore::Model const& model) {
  std::vector<VertexType> verts;
  std::vector<UINT> indices;

  rcore::Mesh mesh = model.getMesh();

  for (size_t i = 0; i < mesh.vertices.size(); i++) {
    verts.push_back({
      mesh.vertices.at(i),
      mesh.normals.at(i),
      mesh.uvs.at(i)
    });

    indices.push_back(static_cast<UINT>(i));
  }

  m_SIVBuffer->createBuffers(verts, indices);
}