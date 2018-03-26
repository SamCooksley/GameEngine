namespace engine
{
  template <class T>
  std::shared_ptr<T> GameObject::AddComponent()
  {
    return AddComponentInternal<T>();
  }

  template <class T>
  std::shared_ptr<T> GameObject::getComponent()
  {
    return getComponentInternal<T>();
  }

  template <class T>
  void GameObject::getComponents(std::vector<std::shared_ptr<T>> & _outComponents)
  {
    getComponentsInternal<T>(_outComponents);
  }

  template <class T>
  std::shared_ptr<T> GameObject::AddComponentInternal()
  {
    static_assert(std::is_base_of<Component, T>::value, "Object must be type of Component");
    
    auto component = std::make_shared<T>();

    m_components.push_back(component);
    component->m_gameObject = getShared();
    component->Awake();

    component->setEnabled(true);

    return component;
  }

  template <class T>
  std::shared_ptr<T> GameObject::getComponentInternal()
  {
    static_assert(std::is_base_of<Component, T>::value, "Object must be type of Component");

    for (auto & component : m_components)
    {
      std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(component);
      if (c) { return c; }
    }

    return nullptr;
  }

  template <class T>
  void GameObject::getComponentsInternal(std::vector<std::shared_ptr<T>> & _outComponents)
  {
    static_assert(std::is_base_of<Component, T>::value, "Object must be type of Component");

    _outComponents.clear();

    for (auto & component : m_components)
    {
      std::shared_ptr<T> c = std::dynamic_pointer_cast<T>(component);

      if (c) { _outComponents.push_back(c); }
    }
  }
}