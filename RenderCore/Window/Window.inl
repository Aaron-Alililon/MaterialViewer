namespace rcore {

  template<std::derived_from<Layer> T, class... Args>
  void Window::addLayer(Args&&... args) {
    m_layers.push_back(std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...));
  }

  template<std::derived_from<Layer> T>
  std::weak_ptr<T> Window::getLayer() {
    for (auto& layer : m_layers) {
      if (auto casted = std::dynamic_pointer_cast<T>(layer)) {
        return casted;
      }
    }
    return {};
  }

}