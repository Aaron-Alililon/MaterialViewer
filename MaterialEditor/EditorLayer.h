#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Layer.h"
#include "Rcore.h"

class EditorLayer : public rcore::Layer {
public:
  virtual void setup() override;
  virtual void update(rcore::FrameState const& frame) override;
  virtual void render(rcore::FrameState const& frame) override;
};

#endif