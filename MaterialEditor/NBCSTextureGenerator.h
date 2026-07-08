#ifndef NBCS_TEXTURE_GENERATOR_H
#define NBCS_TEXTURE_GENERATOR_H

#include "Window.h"
#include "Preset3D.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "RenderTarget.h"
#include "GLTFLoader.h"
#include "ModelSphere.h"
#include "Model.h"

class NBCSTextureGenerator {
private:
  struct __declspec(align(16)) EmptyMaterialProperties {};

public:
  NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<ModelSphere::ExtendedVertexType>> sivBuffer);
  std::pair<ID3D11ShaderResourceView*, ID3D11ShaderResourceView*> getSRVs() const;

private:
  bool prepareTextures();
  std::shared_ptr<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>> material() const;
  void bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<ModelSphere::ExtendedVertexType>> sivBuffer);

private:
  UINT m_textureWidth = 512;
  UINT m_textureHeight = 512;
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
  std::unique_ptr<rcore::RenderTarget> m_tangentTexture;
  std::unique_ptr<rcore::RenderTarget> m_binormalTexture;
};

#endif