#include "pch.h"
#include "Layer.h"
#include "Window.h"

namespace rcore {

  Layer::Layer(std::weak_ptr<Window> const& window) : m_window{ window } {}

}