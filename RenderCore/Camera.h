#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

namespace rcore {

  class Camera {
  public:
    Camera() = default;
    Camera(float x, float y, float z);
    Camera(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& rotation);

  public:
    DirectX::XMFLOAT3 getPosition() const;
    void setPosition(float x, float y, float z);
    void setPosition(DirectX::XMFLOAT3 position);

    DirectX::XMFLOAT3 getRotation() const;
    void setRotation(float pitch, float yaw, float roll);
    void setRotation(DirectX::XMFLOAT3 rotation);

    DirectX::XMMATRIX getViewMatrix() const;
    DirectX::XMMATRIX getPerspectiveMatrix(float aspect) const;

  public:
    float fov = std::numbers::pi_v<float> / 4.0f;
    float nearPlane = 0.3f;
    float farPlane = 1000.f;

  private:
    Transform m_transform{};
  };

}

#endif