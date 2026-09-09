#include "Core/pch.h"
#include "D3D11/DeviceDesc.h"

namespace rcore {

  void DeviceDesc::featureLevels(std::vector<D3D_FEATURE_LEVEL> featureLevels) {
    m_featureLevels = std::move(featureLevels);
  }

  std::vector<D3D_FEATURE_LEVEL> DeviceDesc::featureLevels() const {
    return m_featureLevels;
  }

}