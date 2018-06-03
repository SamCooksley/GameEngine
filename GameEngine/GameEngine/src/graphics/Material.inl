namespace engine {
namespace graphics {

  template <typename T>
  Material::Error Material::setUniform(const String & _name, const T & _value)
  {
    ShaderUniform uniform;
    if (!m_shader->getUniform(_name, &uniform))
    {
      return Error::INVALID_UNIFORM;
    }
  
    return setUniform<T>(uniform, _value);
  }
  
  template <typename T>
  Material::Error Material::getUniform(const String & _name, T * _outValue) const
  {
    ShaderUniform uniform;
    if (!m_shader->getUniform(_name, &uniform))
    {
      return Error::INVALID_UNIFORM;
    }
  
    return getUniform<T>(uniform, _outValue);
  }
  
  template <typename T>
  Material::Error Material::setUniform(const ShaderUniform & _uniform, const T & _value)
  {
    if (TypeData<T>::type != _uniform.type)
    {
      return Error::INVALID_TYPE;
    }
  
    assert(_uniform.offset + _uniform.size <= m_uniformData.size());
  
    memcpy(&m_uniformData[_uniform.offset], &_value, _uniform.size);
  
    return Error::OK;
  }
  
  template <typename T>
  Material::Error Material::getUniform(const ShaderUniform & _uniform, T * _outValue) const
  {
    if (TypeData<T>::type != _uniform.type)
    {
      return Error::INVALID_TYPE;
    }
  
    assert(_uniform.offset + _uniform.size <= m_uniformData.size());
  
    if (_outValue != nullptr)
    {
      memcpy(_outValue, &m_uniformData[_uniform.offset], _uniform.size);
    }
  
    return Error::OK;
  }
  
} } // engine::graphics