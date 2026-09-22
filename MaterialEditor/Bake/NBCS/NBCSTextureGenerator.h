#ifndef NBCS_TEXTURE_GENERATOR_H
#define NBCS_TEXTURE_GENERATOR_H

#include "Window/Window.h"
#include "Core/Preset3D.h"
#include "D3D11/State/DepthStencilState.h"
#include "D3D11/State/RasterizerState.h"
#include "Resource/RenderTarget.h"
#include "Model/Model.h"
#include "Render/Material.h"
#include "Bake/BakePass.h"

class NBCSTextureGenerator : public BakePass {
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

protected:
  virtual bool setupRenderTargets() override;
  virtual bool bindSourceData() override;
  virtual bool draw() override;

private:
  std::vector<std::pair<float, float>> computeTangentScales(std::vector<rcore::Preset3D::StandardVertexType> const& vertices, std::vector<uint32_t> const& indices) const;
  bool makeExtendedSIVBuffer(std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> sivBuffer);
  bool makeMaterial();

private:
  const UINT m_textureWidth = 512;
  const UINT m_textureHeight = 512;

  std::weak_ptr<rcore::StaticIndexedVertexBuffer<rcore::Preset3D::StandardVertexType>> m_sivBuffer;
  std::shared_ptr<rcore::StaticIndexedVertexBuffer<ExtendedVertexType>> m_extendedSIVBuffer;
  std::shared_ptr<rcore::Material<NBCSTextureGenerator::EmptyMaterialProperties>> m_material;
  std::shared_ptr<rcore::RenderTarget> m_normalTexture, m_tangentTexture;
};

#endif