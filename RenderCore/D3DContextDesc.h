#ifndef D3D11_CONTEXT_DESC_H
#define D3D11_CONTEXT_DESC_H

namespace rcore {

  class D3DContextDesc {
  public:
    void targetFps(unsigned int targetFps);
    unsigned int targetFps() const;

    void fov(float fov);
    float fov() const;

    void nearPlane(float nearPlane);
    float nearPlane() const;

    void farPlane(float farPlane);
    float farPlane() const;

  private:
    unsigned int m_targetFps = 60;
    float m_fov = 3.141592654f / 4.0f;
    float m_near = 0.3f;
    float m_far = 1000.f;
  };

}

#endif