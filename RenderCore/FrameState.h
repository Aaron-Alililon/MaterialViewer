#ifndef FRAME_STATE_H
#define FRAME_STATE_H

namespace rcore {

  struct FrameState {
    int width = 1;
    int height = 1;
    int frameCount = 0;
    double dTime = 0;
  };

}

#endif