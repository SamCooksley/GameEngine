namespace engine
{
  namespace graphics
  {
    template <typename T>
    bool Shader::AddUniform(const std::string & _name, ShaderUniform * _outUniform)
    {
      return AddUniform(_name, TypeData<T>::type, sizeof(T), _outUniform);
    }

    template <typename T>
    bool Shader::AddAttribute(const std::string & _name, bool _normalized, ShaderAttribute * _outAttribute)
    {
      return AddAttribute(_name, TypeData<T>::component, TypeData<T>::count, _normalized, _outAttribute);
    }
  }
}