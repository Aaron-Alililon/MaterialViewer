namespace rcore {

  template<std::derived_from<Layer> T>
  void WindowView::addLayer() const {
    if (isValid()) {
      m_window->addLayer<T>();
    }
  }

}