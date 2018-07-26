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

  template <class T>
  Material::Error Material::setTexture(const String & _name, const std::shared_ptr<T> & _texture, bool _new)
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of texture");

    ShaderSampler sampler;
    if (!m_shader->getSampler(_name, &sampler))
    {
      return Error::INVALID_UNIFORM;
    }

    return setTexture(sampler, T::type, _texture, _new);
  }

  template <class T>
  Material::Error Material::getTexture(const String & _name, std::shared_ptr<T> * _outTexture) const
  {
    static_assert(std::is_base_of<Texture, T>::value, "Object must be type of texture");

    if (_outTexture != nullptr)
    {
      *_outTexture = nullptr;
    }

    ShaderSampler sampler;
    if (!m_shader->getSampler(_name, &sampler))
    {
      return Error::INVALID_UNIFORM;
    }

    if (sampler.type != T::type)
    {
      return Error::INVALID_TYPE;
    }

    int unit;
    Error error = getTextureUnit(_name, &unit);
    if (error != Error::OK)
    {
      return error;
    }

    if (unit < 0)
    {
      return Error::OK;
    }

    auto texture = m_textures.find(unit);
    assert(texture != m_textures.mend());

    if (_outTexture != nullptr)
    {
      *_outTexture = std::static_pointer_cast<T>(m_textures[texture->second].texture);
    }

    return Error::OK;
  }
  
} } // engine::graphics