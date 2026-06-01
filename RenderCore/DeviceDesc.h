#ifndef DEVICE_DESC_H
#define DEVICE_DESC_H

#include "pch.h"

namespace rcore {

  class DeviceDesc {
  public:
    void featureLevels(std::vector<D3D_FEATURE_LEVEL> featureLevels);
    std::vector<D3D_FEATURE_LEVEL> featureLevels() const;

  private:
    
    std::vector<D3D_FEATURE_LEVEL> m_featureLevels{
      D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0
    };
  };

}

#endif