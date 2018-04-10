namespace engine
{
  template <class T>
  std::shared_ptr<T> Resources::Load(const std::string & _path)
  {
    static_assert(std::is_base_of<Object, T>::value, "Resource must be type of Object.");

    auto res = Application::s_context->resources.find(_path);
    if (res != Application::s_context->resources.end())
    {
      return std::dynamic_pointer_cast<T>(res->second);
    }

    auto asset = T::Load(_path);

    if (!asset)
    {
      return nullptr;
    }

    Application::s_context->resources[_path] = asset;
    return asset;
  }
}