#include "stdafx.h"

#include "Material.h"

#include "Graphics.h"

namespace engine {
namespace graphics {
  
  Material::Material(const std::shared_ptr<Shader> & _shader) :
    m_shader(_shader)
  {
    if (!m_shader)
    {
      throw std::invalid_argument("null shader");
    }

    m_shader->RetreiveUniformData(m_uniformData);

    // TODO: default texture - has to be one for each texture type
    for (auto & sampler : m_shader->m_samplers)
    {
      // would be better to use setTextureUnit(ShaderSampler, int) however, 
      // the unit will not exist meaning that it will fail.
      setUniform<int>(sampler.name, -1);
    }
  }
  
  Material::~Material()
  { }
  
  void Material::Bind() const
  {
    m_shader->Bind();
  
    for (auto & uniform : m_shader->m_uniforms)
    {
      m_shader->setUniform(uniform.location, uniform.type, &m_uniformData[uniform.offset]);
    }
  
    for (auto & unit : m_textures)
    {
      if (unit.texture)
      {
        unit.texture->Bind(unit.unit);
      }
    }
  }
  
  Material::Error Material::setTexture(const ShaderSampler & _sampler, TextureType _type, const std::shared_ptr<Texture> & _texture, bool _new)
  {
    if (_sampler.type != _type)
    {
      return Error::INVALID_TYPE;
    }
  
    int unit;
    Error error = getTextureUnit(_sampler, &unit);
    if (error != Error::OK)
    {
      return error;
    }
  
    auto texture = m_textures.find(unit);
    if (!_new && texture != m_textures.mend())
    {
      assert(m_textures[texture->second].type == _type);
  
      m_textures[texture->second].texture = _texture;
    }
    else
    {
      TextureUnit texture;
      texture.count = 0;
      texture.unit = FindFreeTextureUnit();
      texture.type = _type;
      texture.texture = _texture;
  
      m_textures.add(texture.unit, texture);
  
      error = setTextureUnit(_sampler, texture.unit);
      if (error != Error::OK)
      {
        return error;
      }
    }
  
    return Error::OK;
  }
  
  Material::Error Material::setTextureUnit(const String & _name, int _unit)
  {
    ShaderSampler sampler;
    if (!m_shader->getSampler(_name, &sampler))
    {
      return Error::INVALID_UNIFORM;
    }
  
    return setTextureUnit(sampler, _unit);
  }
  
  Material::Error Material::setTextureUnit(const ShaderSampler & _sampler, int _unit)
  {
    auto textureIter = m_textures.find(_unit);
    if (textureIter == m_textures.mend())
    {
      return Error::INVALID_TEXTURE_UNIT;
    }
  
    TextureUnit & texture = m_textures[textureIter->second];
  
    if (texture.type != _sampler.type)
    {
      return Error::INVALID_TYPE;
    }
  
    int oldUnit;
    Error error = getTextureUnit(_sampler, &oldUnit);
    if (error != Error::OK)
    {
      return error;
    }
  
    if (oldUnit == _unit)
    {
      return Error::OK;
    }
  
    //remove the reference from the old unit.
    auto oldTexture = m_textures.find(oldUnit);
    if (oldTexture != m_textures.mend())
    {
      if (--m_textures[oldTexture->second].count <= 0)
      {
        //if there are no more references, delete the unit.
        m_textures.remove(oldTexture);
      }
    }
  
    error = setUniform<int>(m_shader->m_uniforms[_sampler.uniformIndex], _unit);
    if (error != Error::OK)
    {
      return error;
    }
    ++texture.count;
  
    return Error::OK;
  }
  
  Material::Error Material::getTextureUnit(const String & _name, int * _unit) const
  {
    ShaderSampler sampler;
    if (!m_shader->getSampler(_name, &sampler))
    {
      return Error::INVALID_UNIFORM;
    }
  
    return getTextureUnit(sampler, _unit);
  }
  
  Material::Error Material::getTextureUnit(const ShaderSampler & _sampler, int * _unit) const
  {
    return getUniform<int>(m_shader->m_uniforms[_sampler.uniformIndex], _unit);
  }
  
  int Material::FindFreeTextureUnit() const
  {
    int unit = 0;
    while (m_textures.exists(unit))
    {
      ++unit;
    }
    return unit;
  }
  
  const std::shared_ptr<Shader> & Material::getShader()
  {
    return m_shader;
  }

} } // engine::graphics