#include "CameraController.h"

CameraController::CameraController() : m_cam{ 0, 0, -10 } {
  m_cam.farPlane = 10000.0f;
}

void CameraController::setMatrices(std::weak_ptr<rcore::MatrixBuffer> matrixBuffer, float resolution) {
  auto lockedMatrixBuffer = matrixBuffer.lock();
  if (!lockedMatrixBuffer) return;

  float x = m_radius * cosf(m_elevation) * sinf(m_azimuth);
  float y = m_radius * sinf(m_elevation);
  float z = m_radius * cosf(m_elevation) * cosf(m_azimuth);

  m_cam.setPosition(x, y, z);
  m_cam.setLookAt(0, 0, 0);

  lockedMatrixBuffer->setProjectionMatrix(m_cam.getPerspectiveMatrix(resolution));
  lockedMatrixBuffer->setViewMatrix(m_cam.getViewMatrix());
}

void CameraController::onEvent(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
  switch (umsg) {
    case WM_MOUSEWHEEL: {
      float scroll = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA;
      m_radius = max(0.1f, m_radius - scroll * 0.5f);

      break;
    }

    case WM_MBUTTONDOWN: {
      m_middleMouseHeld = true;
      m_lastMousePos = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
      SetCapture(hwnd);

      break;
    }

    case WM_MBUTTONUP: {
      m_middleMouseHeld = false;
      ReleaseCapture();

      break;
    }

    case WM_MOUSEMOVE: {
      if (m_middleMouseHeld) {
        float dx = static_cast<float>(GET_X_LPARAM(lparam) - m_lastMousePos.x);
        float dy = static_cast<float>(GET_Y_LPARAM(lparam) - m_lastMousePos.y);
        m_lastMousePos = { GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };

        m_azimuth += dx * 0.005f;
        m_elevation += dy * 0.005f;

        m_elevation = max(-DirectX::XM_PIDIV2 + 0.01f, min(DirectX::XM_PIDIV2 - 0.01f, m_elevation));
      }
      break;
    }
  }
}