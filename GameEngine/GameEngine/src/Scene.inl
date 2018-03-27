namespace engine {

  template <class T>
  void Scene::SetScene()
  {
    Application::s_context->nextScene = std::make_shared<T>();
  }

}