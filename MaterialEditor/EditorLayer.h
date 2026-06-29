#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"
#include "MatrixBuffer.h"
#include "Camera.h"
#include "ModelSphere.h"
#include "Preset3D.h"

class EditorLayer : public rcore::Layer {

public:
  EditorLayer(std::weak_ptr<rcore::Window> window, rcore::D3DContextDesc contextDesc);

public:
  void update(rcore::FrameState const& frame) override;
  void render(rcore::FrameState const& frame) override;

private:
  rcore::D3DContextDesc m_ctxDesc;
  rcore::Camera m_cam;
  std::shared_ptr<rcore::MatrixBuffer> m_matrixBuffer;
  std::unique_ptr<ModelSphere> m_modelSphere;
};

#endif