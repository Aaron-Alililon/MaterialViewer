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

  struct ExtendedVertexType : rcore::Preset3D::StandardVertexType {
    float sTangent;
    float sBinormal;
  };

public:
  NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer);
  std::pair<ID3D11ShaderResourceView*, ID3D11ShaderResourceView*> getSRVs() const;

private:
  std::vector<std::pair<float, float>> computeTangentScales(std::vector<rcore::Preset3D::StandardVertexType> const& vertices, std::vector<uint32_t> const& indices);
  rcore::StaticIndexedVertexBuffer<ExtendedVertexType> makeExtendedSIVBuffer(std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer);
  bool prepareTextures();
  std::shared_ptr<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>> material() const;
  void bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window, rcore::StaticIndexedVertexBuffer<ExtendedVertexType> sivBuffer);

private:
  UINT m_textureWidth = 1024;
  UINT m_textureHeight = 1024;
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
  std::unique_ptr<rcore::RenderTarget> m_normalTexture, m_tangentTexture;
};

#endif