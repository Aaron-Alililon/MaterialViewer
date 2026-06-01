#include "pch.h"
#include "D3DContextDesc.h"

namespace rcore {

  void D3DContextDesc::targetFps(unsigned int targetFps) {
    m_targetFps = targetFps;
  }

  unsigned int D3DContextDesc::targetFps() const {
    return m_targetFps;
  }

  void D3DContextDesc::fov(float fov) {
    m_fov = fov;
  }

  float D3DContextDesc::fov() const {
    return m_fov;
  }

  void D3DContextDesc::nearPlane(float nearPlane) {
    m_near = nearPlane;
  }

  float D3DContextDesc::nearPlane() const {
    return m_near;
  }

  void D3DContextDesc::farPlane(float farPlane) {
    m_far = farPlane;
  }

  float D3DContextDesc::farPlane() const {
    return m_far;
  }

}