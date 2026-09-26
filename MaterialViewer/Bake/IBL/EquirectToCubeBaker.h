#ifndef EQUIRECT_TO_CUBE_BAKER_H
#define EQUIRECT_TO_CUBE_BAKER_H

#include "Resource/RenderTarget.h"
#include "Resource/Texture.h"
#include "Resource/Sampler.h"
#include "Resource/HDRLoader.h"
#include "Render/Material.h"
#include "Render/Shader.h"
#include "D3D11/Buffer/CBuffer.h"
#include "Bake/BakePass.h"

class EquirectToCubeBaker : public BakePass {
private:
  struct __declspec(align(16)) EmptyMaterialProperties {};

  struct __declspec(align(16)) FaceDirectionData {
    DirectX::XMFLOAT3 forward; float _pad0;
    DirectX::XMFLOAT3 up;      float _pad1;
    DirectX::XMFLOAT3 right;   float _pad2;
  };

public:
  EquirectToCubeBaker(std::weak_ptr<rcore::Window> const& window, std::string const& texturePath);

public:
  std::shared_ptr<rcore::RenderTarget> getEnvironmentCube() const;

protected:
  virtual std::string makeCacheKey() const override;
  virtual std::vector<CacheEntry> getCacheEntries() override;
  virtual bool setupRenderTargets() override;
  virtual bool bindSourceData() override;
  virtual bool draw() override;

private:
  bool loadTexture();
  bool makeFaceBuffer();
  bool makeMaterial();

private:
  const UINT m_textureWidth = 1024;
  const UINT m_textureHeight = 1024;

  std::string m_texturePath;
  std::shared_ptr<rcore::RenderTarget> m_envCube;
  rcore::Texture m_texture;
  rcore::Sampler m_sampler;
  std::shared_ptr<rcore::Material<EmptyMaterialProperties>> m_material;
  std::unique_ptr<rcore::CBuffer<FaceDirectionData>> m_faceBuffer;
};

#endif