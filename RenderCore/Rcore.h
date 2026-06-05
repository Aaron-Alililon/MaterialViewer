#ifndef RCORE_H
#define RCORE_H

#include "D3D11Device.h"
#include "D3D11Context.h"
#include "Engine.h"
#include "Window.h"
#include "WindowView.h"

namespace rcore {

  inline void initDevice(DeviceDesc& deviceDescriptor) {
    D3D11Device::initialize(deviceDescriptor);
  }

  inline void init() {
    Engine::get().run();
  }

  inline WindowView makeWindow(WindowDesc const& descriptor) {
    auto uPtrWindow = std::make_unique<Window>(descriptor);
    Window* rawPtrWindow = uPtrWindow.get();
    Engine::get().registerWindow(std::move(uPtrWindow), !Engine::get().isRunning());

    return { rawPtrWindow };
  }

  inline void makeD3D11Context(WindowView const& window, D3DContextDesc const& descriptor) {
    window.raw()->setContext(std::make_unique<D3D11Context>(window, descriptor));
  }

}

#endif