#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Camera.h"
#include "MatrixBuffer.h"

class CameraController {
public:
  CameraController();

public:
  void setMatrices(std::weak_ptr<rcore::MatrixBuffer> matrixBuffer, float resolution);
  void updatePosition();
  bool onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

  DirectX::XMFLOAT3 getPosition() const;

private:
  float m_radius = 10.0f;
  float m_azimuth = 2.0f;
  float m_elevation = 0.0f;
  rcore::Camera m_cam{};
  bool m_middleMouseHeld = false;
  DirectX::XMINT2 m_lastMousePos{};
};

#endif