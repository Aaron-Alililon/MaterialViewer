#ifndef NBCS_TEXTURE_GENERATOR_H
#define NBCS_TEXTURE_GENERATOR_H

#include "Window/Window.h"
#include "Core/Preset3D.h"
#include "D3D11/State/DepthStencilState.h"
#include "D3D11/State/RasterizerState.h"
#include "Resource/RenderTarget.h"
#include "Model/Model.h"
#include "Render/Material.h"

class NBCSTextureGenerator {
private:
  struct __declspec(align(16)) EmptyMaterialProperties {};

  struct ExtendedVertexType : rcore::Preset3D::StandardVertexType {
    float sTangent;
    float sBinormal;
  };

public:
  NBCSTextureGenerator(std::weak_ptr<rcore::Window> const& window, std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer);

public:
  std::pair<std::shared_ptr<rcore::RenderTarget>, std::shared_ptr<rcore::RenderTarget>> getTextures() const;

private:
  std::vector<std::pair<float, float>> computeTangentScales(std::vector<rcore::Preset3D::StandardVertexType> const& vertices, std::vector<uint32_t> const& indices) const;
  bool makeExtendedSIVBuffer(std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer);
  bool makeMaterial();
  bool prepareTextures();
  void bakeTangentBinormalTextures(std::weak_ptr<rcore::Window> const& window);

private:
  const UINT m_textureWidth = 512;
  const UINT m_textureHeight = 512;
  rcore::DepthStencilState m_depthStencilState;
  rcore::RasterizerState m_rasterizerState;
  std::shared_ptr<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>> m_material;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<ExtendedVertexType>> m_extendedSIVBuffer;
  std::shared_ptr<rcore::RenderTarget> m_normalTexture, m_tangentTexture;
};

#endif