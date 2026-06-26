#include "pch.h"
#include "Transform.h"

namespace rcore {

  DirectX::XMMATRIX Transform::getWorldMatrix() const {
    return 
      DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
      DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
      DirectX::XMMatrixTranslation(position.x, position.y, position.z);
  }

}