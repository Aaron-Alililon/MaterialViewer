#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace rcore {

  struct Transform {
    DirectX::XMFLOAT3 position{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 rotation{ 0.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 scale{ 1.0f, 1.0f, 1.0f };

    DirectX::XMMATRIX getWorldMatrix() const;
  };

}

#endif