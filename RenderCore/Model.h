#ifndef MODEL_H
#define MODEL_H

#include "Transform.h"
#include "MeshLoader.h"
#include "MaterialBase.h"
#include "D3D11Device.h"
#include "MatrixBuffer.h"
#include "VertexBufferBase.h"

namespace rcore {

  class Model {
  public:
    Model() = default;
    Model(Mesh const& mesh, Transform const& transform, std::shared_ptr<MaterialBase> const& material, std::shared_ptr<VertexBufferBase> const& vertexBuffer);
    Model(std::string meshFile, Transform const& transform, std::shared_ptr<MaterialBase> const& material, std::shared_ptr<VertexBufferBase> const& vertexBuffer);

  public:
    void drawIndexed(MatrixBuffer& matrixBuffer);

    DirectX::XMFLOAT3 getPosition() const;
    void setPosition(float x, float y, float z);
    void setPosition(DirectX::XMFLOAT3 position);

    DirectX::XMFLOAT3 getRotation() const;
    void setRotation(float pitch, float yaw, float roll);
    void setRotation(DirectX::XMFLOAT3 rotation);

    DirectX::XMFLOAT3 getScale() const;
    void setScale(float x, float y, float z);
    void setScale(DirectX::XMFLOAT3 scale);

    Mesh getMesh() const;

  private:
    bool m_valid = false;
    Transform m_transform{};
    Mesh m_mesh{};
    std::shared_ptr<MaterialBase> m_material;
    std::shared_ptr<VertexBufferBase> m_vertexBuffer;
  };

}

#endif