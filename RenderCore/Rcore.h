#ifndef RCORE_H
#define RCORE_H

#include "D3D11Device.h"
#include "Engine.h"
#include "Window.h"
#include "WindowView.h"

namespace rcore {

  inline void init(DeviceDesc& deviceDescriptor) {
    D3D11Device::initialize(deviceDescriptor);
    Engine::get().run();
  }

  inline WindowView makeWindow(WindowDesc const& descriptor) {
    auto uPtrWindow = std::make_unique<Window>(descriptor);
    Window* rawPtrWindow = uPtrWindow.get();
    Engine::get().registerWindow(std::move(uPtrWindow), !Engine::get().isRunning());

    return { rawPtrWindow };
  }

}

#endif