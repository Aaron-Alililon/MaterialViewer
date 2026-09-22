#ifndef BRDF_LUT_BAKER_H
#define BRDF_LUT_BAKER_H

#include "Resource/RenderTarget.h"
#include "Render/Material.h"
#include "Bake/BakePass.h"

class BRDFLUTBaker : public BakePass {
private:
  struct __declspec(align(16)) EmptyMaterialProperties {};

public:
  BRDFLUTBaker(std::weak_ptr<rcore::Window> const& window);

public:
  std::shared_ptr<rcore::RenderTarget> getLUT() const;

protected:
  virtual bool setupRenderTargets() override;
  virtual bool bindSourceData() override;
  virtual bool draw() override;

private:
  bool makeMaterial();

private:
  const UINT m_size = 256;

  std::shared_ptr<rcore::RenderTarget> m_lut;
  std::shared_ptr<rcore::Material<EmptyMaterialProperties>> m_material;
};

#endif