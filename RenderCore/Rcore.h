#include "pch.h"

#ifndef RCORE_H
#define RCORE_H

#include "D3D11Device.h"
#include "D3D11Context.h"
#include "Engine.h"
#include "Window.h"

namespace rcore {

  inline void initDevice(DeviceDesc& deviceDescriptor) {
    D3D11Device::initialize(deviceDescriptor);
  }

  inline void init() {
    Engine::get().run();
  }

  inline std::weak_ptr<Window> makeWindow(WindowDesc const& descriptor) {
    auto sPtrWindow = std::make_shared<Window>(descriptor);
    std::weak_ptr<Window> wPtrWindow = sPtrWindow;

    Engine::get().registerWindow(std::move(sPtrWindow), !Engine::get().isRunning());

    return wPtrWindow;
  }

  inline void makeD3D11Context(std::weak_ptr<Window> window, D3DContextDesc const& descriptor) {
    auto lockedWindow = window.lock();
    if (!lockedWindow) {
      RCORE_LOG(ERR, "Tried creating D3D11Context with expired window");
      return;
    }

    lockedWindow->setContext(std::make_unique<D3D11Context>(lockedWindow, descriptor));
  }

}

#endif