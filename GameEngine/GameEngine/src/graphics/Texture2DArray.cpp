#include "stdafx.h"

#include "Texture2DArray.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  Texture2DArray::Texture2DArray(TextureFormat _format, int _width, int _height, int _layers, int _mipmaps) :
    Texture(TextureType::TEXTURE_2D_ARRAY)
  {
    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = _layers;

    m_mipmaps = _mipmaps;
    if (m_mipmaps <= 0)
    {
      m_mipmaps = CalculateMipMapCount(_width, _height);
    }

    glGenTextures(1, &m_id);
    Bind();

    glTexStorage3D(
      GL_TEXTURE_2D_ARRAY, _mipmaps,
      TextureFormatToOpenGL(m_format),
      m_width, m_height, m_depth
    );

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::LINEAR);
  }

  Texture2DArray::~Texture2DArray()
  {
    glDeleteTextures(1, &m_id);
  }

  void Texture2DArray::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
  }

  void Texture2DArray::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    Bind();
  }

  void Texture2DArray::setWrap(TextureWrap _wrap)
  {
    m_wrap = _wrap;
    GLenum wrap = TextureWrapToOpenGL(m_wrap);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap);
  }

  void Texture2DArray::setFilter(TextureFilter _filter)
  {
    m_filter = _filter;
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(m_filter, true));
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(m_filter, false));
  }

  void Texture2DArray::setBorder(const glm::vec4 & _colour)
  {
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_colour));
  }

  void Texture2DArray::setAnisotropy(float _anisotropy)
  {
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, glm::min(_anisotropy, Graphics::GL().getMaxAnisotropy()));
  }

  void Texture2DArray::GenerateMipMaps()
  {
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  }

} } // engine::graphics