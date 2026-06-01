#ifndef D3D11_DEVICE_H
#define D3D11_DEVICE_H

#include "pch.h"
#include "DeviceDesc.h"

namespace rcore {

  class D3D11Device {
  private:
    static D3D11Device* instance;

  public:
    static void initialize(DeviceDesc& deviceDescriptor);
    static D3D11Device& get();

  private:
    D3D11Device(DeviceDesc& deviceDescriptor);

  public:
    ID3D11Device* raw() const;
    ID3D11DeviceContext* rawContext() const;
    D3D_FEATURE_LEVEL featureLevel() const;

  private:
    void createDevice(DeviceDesc& deviceDescriptor);

  private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    D3D_FEATURE_LEVEL m_featureLevel;
  };

}

#endif