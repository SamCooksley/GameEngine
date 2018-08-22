#include "stdafx.h"

#include "Texture.h"

namespace engine {
namespace graphics {
  
  int Texture::CalculateMipMapCount(int _width, int _height)
  {
    float width = static_cast<float>(_width);
    float height = static_cast<float>(_height);
    return static_cast<int>(glm::floor(glm::log2(glm::max(width, height)))) + 1;
  }

  Texture::Texture(TextureType _type) :
    m_id(0), m_type(_type), m_format(TextureFormat::NONE), 
    m_width(1), m_height(1), 
    m_depth(1), m_mipmaps(1),
    m_filter(TextureFilter::LINEAR),
    m_wrap(TextureWrap::REPEAT)
  { }
  
  Texture::~Texture()
  { }

  int Texture::getWidth() const
  {
    return m_width;
  }

  int Texture::getHeight() const
  {
    return m_height;
  }

  int Texture::getDepth() const
  {
    return m_depth;
  }

  int Texture::getMipmapCount() const
  {
    return m_mipmaps;
  }

  int Texture::getMipMapWidth(int _level) const
  {
    return m_width / glm::pow(2, _level);
  }

  int Texture::getMipMapHeight(int _level) const
  {
    return m_height / glm::pow(2, _level);
  }

  TextureType Texture::getType() const
  {
    return m_type;
  }

  TextureFormat Texture::getFormat() const
  {
    return m_format;
  }

  TextureFilter Texture::getFilter() const
  {
    return m_filter;
  }

  TextureWrap Texture::getWrap() const
  {
    return m_wrap;
  }

} } // engine::graphics