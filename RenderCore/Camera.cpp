#include "pch.h"
#include "Camera.h"

namespace rcore {

  Camera::Camera(float x, float y, float z) : m_transform{ { x, y, z } } {}

  Camera::Camera(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& rotation) : m_transform{ position, rotation } {}

  DirectX::XMFLOAT3 Camera::getPosition() const {
    return m_transform.position;
  }

  void Camera::setPosition(float x, float y, float z) {
    setPosition({ x, y, z });
  }

  void Camera::setPosition(DirectX::XMFLOAT3 position) {
    m_transform.position = position;
  }

  DirectX::XMFLOAT3 Camera::getRotation() const {
    return m_transform.rotation;
  }

  void Camera::setRotation(float pitch, float yaw, float roll) {
    setRotation({ pitch, yaw, roll });
  }

  void Camera::setRotation(DirectX::XMFLOAT3 rotation) {
    m_transform.rotation = rotation;
  }

  DirectX::XMMATRIX Camera::getViewMatrix() const {
    return DirectX::XMMatrixInverse(nullptr, m_transform.getWorldMatrix());
  }

  DirectX::XMMATRIX Camera::getPerspectiveMatrix(float aspect) const {
    return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane);
  }

}