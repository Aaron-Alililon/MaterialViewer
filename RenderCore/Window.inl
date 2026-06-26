namespace rcore {

  template<std::derived_from<Layer> T, class... Args>
  void Window::addLayer(Args... args) {
    m_layers.push_back(std::make_unique<T>(weak_from_this(), std::forward<Args>(args)...));
  }

}