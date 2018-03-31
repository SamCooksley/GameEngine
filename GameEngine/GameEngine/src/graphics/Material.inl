namespace engine
{
  namespace graphics
  {
    template <typename T>
    void Material::setUniform(const std::string & _name, const T & _value)
    {
      setUniform(_name, &_value, sizeof(T), TypeData<T>::type);
    }

    template <typename T>
    bool Material::getUniform(const std::string & _name, T * _outValue) const
    {
      return getUniform(_name, _outValue, sizeof(T), TypeData<T>::type);
    }
  }
}