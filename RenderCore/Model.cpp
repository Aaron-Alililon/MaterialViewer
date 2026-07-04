#include "pch.h"
#include "Model.h"

namespace rcore {

  Model::Model(std::shared_ptr<MaterialBase> const& material, std::shared_ptr<IVertexBuffer> const& vertexBuffer) : m_material{ material }, m_vertexBuffer{ vertexBuffer } {
    if (m_material &&
        m_material->valid() &&
        m_vertexBuffer &&
        m_vertexBuffer->valid()
    ) {
      m_valid = true;
    }
  }

  void Model::drawIndexed(MatrixBuffer& matrixBuffer, bool activateMaterial) {
    if (!m_valid) {
      RCORE_LOG(WARN, "Tried drawing invalid model");
      return;
    }

    // Sophisticated renderers will batch models by material and only activate once
    if (activateMaterial) {
      m_material->activate();
    }

    matrixBuffer.setWorldMatrix(m_transform.getWorldMatrix());
    matrixBuffer.uploadBuffer();

    UINT indexCount = m_vertexBuffer->bind();

    D3D11Device::get().rawContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    D3D11Device::get().rawContext()->DrawIndexed(indexCount, 0, 0);
  }

  Transform Model::getTransform() const {
    return m_transform;
  }

  void Model::setTransform(Transform const& transform) {
    m_transform = transform;
  }

  DirectX::XMFLOAT3 Model::getPosition() const {
    return m_transform.position;
  }

  void Model::setPosition(float x, float y, float z) {
    setPosition({ x, y, z });
  }

  void Model::setPosition(DirectX::XMFLOAT3 position) {
    m_transform.position = position;
  }

  DirectX::XMFLOAT3 Model::getRotation() const {
    return m_transform.rotation;
  }

  void Model::setRotation(float pitch, float yaw, float roll) {
    setRotation({ pitch, yaw, roll });
  }

  void Model::setRotation(DirectX::XMFLOAT3 rotation) {
    m_transform.rotation = rotation;
  }

  DirectX::XMFLOAT3 Model::getScale() const {
    return m_transform.scale;
  }

  void Model::setScale(float x, float y, float z) {
    setScale({ x, y, z });
  }

  void Model::setScale(DirectX::XMFLOAT3 scale) {
    m_transform.scale = scale;
  }

}