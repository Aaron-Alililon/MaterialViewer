#include "pch.h"
#include "DeviceDesc.h"

namespace rcore {

  void DeviceDesc::featureLevels(std::vector<D3D_FEATURE_LEVEL> featureLevels) {
    m_featureLevels = std::move(featureLevels);
  }

  std::vector<D3D_FEATURE_LEVEL> DeviceDesc::featureLevels() const {
    return m_featureLevels;
  }

}