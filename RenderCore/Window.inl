namespace rcore {

  template<std::derived_from<Layer> T>
  void Window::addLayer() {
    m_layers.push_back(std::make_unique<T>(weak_from_this()));
  }

}