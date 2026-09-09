#include "Core/pch.h"
#include "Core/Layer.h"
#include "Window/Window.h"

namespace rcore {

  Layer::Layer(std::weak_ptr<Window> const& window) : m_window{ window } {}

}