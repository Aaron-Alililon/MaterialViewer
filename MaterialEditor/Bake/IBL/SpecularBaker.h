#ifndef SPECULAR_BAKER_H
#define SPECULAR_BAKER_H

#include "Resource/RenderTarget.h"
#include "Resource/Sampler.h"
#include "Render/Material.h"
#include "Render/Shader.h"
#include "D3D11/Buffer/CBuffer.h"
#include "Bake/BakePass.h"

class SpecularBaker : public BakePass {
private:
  struct __declspec(align(16)) EmptyMaterialProperties {};

  struct __declspec(align(16)) FaceDirectionData {
    DirectX::XMFLOAT3 forward;   float _pad0;
    DirectX::XMFLOAT3 up;        float _pad1;
    DirectX::XMFLOAT3 right;
    float roughness;
  };

public:
  SpecularBaker(std::weak_ptr<rcore::Window> const& window, std::shared_ptr<rcore::RenderTarget> const& envCube);

public:
  std::shared_ptr<rcore::RenderTarget> getPrefilteredCube() const;

protected:
  virtual bool setupRenderTargets() override;
  virtual bool bindSourceData() override;
  virtual bool draw() override;

private:
  bool makeFaceBuffer();
  bool makeMaterial();

private:
  const UINT m_baseWidth = 1024;
  const UINT m_baseHeight = 1024;
  const UINT m_mipCount = 6;

  std::shared_ptr<rcore::RenderTarget> m_prefilteredCube;
  std::shared_ptr<rcore::RenderTarget> m_envCube;
  rcore::Sampler m_sampler;
  std::shared_ptr<rcore::Material<EmptyMaterialProperties>> m_material;
  std::unique_ptr<rcore::CBuffer<FaceDirectionData>> m_faceBuffer;
};

#endif