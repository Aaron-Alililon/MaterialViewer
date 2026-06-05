#include "pch.h"
#include "D3D11Device.h"

namespace rcore {

  D3D11Device* D3D11Device::instance = nullptr;

  void D3D11Device::initialize(DeviceDesc& deviceDescriptor) {
    instance = new D3D11Device(deviceDescriptor);
  }

  D3D11Device& D3D11Device::get() {
    if (instance == nullptr) {
      RCORE_LOG(ERR, "Tried accessing D3D11Device before initialization");
      assert(false && "Check log for more info");
    }

    return *instance;
  }

  D3D11Device::D3D11Device(DeviceDesc& deviceDescriptor) {
    createDevice(deviceDescriptor);
  }

  ID3D11Device* D3D11Device::raw() const {
    return m_device.Get();
  }

  ID3D11DeviceContext* D3D11Device::rawContext() const {
    return m_context.Get();
  }

  D3D_FEATURE_LEVEL D3D11Device::featureLevel() const {
    return m_featureLevel;
  }

  // TODO do dxgi factory setup for exact refresh rates
  void D3D11Device::createDevice(DeviceDesc& deviceDescriptor) {
    std::vector<D3D_FEATURE_LEVEL> featureLevels = deviceDescriptor.featureLevels();

    HRESULT result = D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
#ifdef _DEBUG
      D3D11_CREATE_DEVICE_DEBUG,
#else
      0,
#endif
      featureLevels.data(),
      (UINT)featureLevels.size(),
      D3D11_SDK_VERSION,
      &m_device,
      &m_featureLevel,
      &m_context
    );

    if (FAILED(result)) {
      RCORE_LOG(ERR, "D3D11CreateDevice failed with HRESULT: " + std::to_string(result));
      assert(false && "Check log for more info");
    }
  }

}