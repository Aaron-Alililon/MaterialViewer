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

  void Camera::addPosition(float x, float y, float z) {
    addPosition({ x, y, z });
  }

  void Camera::addPosition(DirectX::XMFLOAT3 position) {
    m_transform.position.x += position.x;
    m_transform.position.y += position.y;
    m_transform.position.z += position.z;
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

  void Camera::addRotation(float pitch, float yaw, float roll) {
    addRotation({ pitch, yaw, roll });
  }

  void Camera::addRotation(DirectX::XMFLOAT3 rotation) {
    m_transform.rotation.x += rotation.x;
    m_transform.rotation.y += rotation.y;
    m_transform.rotation.z += rotation.z;
  }

  void Camera::setLookAt(float x, float y, float z) {
    setLookAt({ x, y, z });
  }

  void Camera::setLookAt(DirectX::XMFLOAT3 focus) {
    DirectX::XMFLOAT3 pos = m_transform.position;

    float dx = focus.x - pos.x;
    float dy = focus.y - pos.y;
    float dz = focus.z - pos.z;

    float pitch = atan2f(dy, sqrtf(dx * dx + dz * dz));
    float yaw = atan2f(dx, dz);

    m_transform.rotation = { -pitch, yaw, 0.0f };
  }

  DirectX::XMMATRIX Camera::getViewMatrix() const {
    return DirectX::XMMatrixInverse(nullptr, m_transform.getWorldMatrix());
  }

  DirectX::XMMATRIX Camera::getPerspectiveMatrix(float aspect) const {
    return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearPlane, farPlane);
  }

}