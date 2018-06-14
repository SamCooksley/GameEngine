#include "stdafx.h"

#include "Texture2DArray.h"

namespace engine {
namespace graphics {

  Texture2DArray::Texture2DArray(uint _width, uint _height, uint _layers, uint _mipmaps, TextureFormat _format) :
    Texture(TextureType::TEXTURE_2D_ARRAY),
    m_width(_width), m_height(_height), m_layers(_layers),
    m_format(_format)
  {
    GLCALL(glGenTextures(1, &m_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, m_id));

    GLCALL(
      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY, _mipmaps,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, m_layers
      )
    );

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::LINEAR);
  }

  Texture2DArray::~Texture2DArray()
  {
    GLCALL(glDeleteTextures(1, &m_id));
  }

  void Texture2DArray::Bind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, m_id));
  }

  void Texture2DArray::Unbind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
  }

  void Texture2DArray::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    GLCALL(
      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY, 1,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, m_layers
      )
    );

  }

  void Texture2DArray::setWrap(TextureWrap _wrap)
  {
    m_wrap = _wrap;
    GLenum wrap = TextureWrapToOpenGL(m_wrap);
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrap));
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrap));
  }

  void Texture2DArray::setFilter(TextureFilter _filter)
  {
    m_filter = _filter;
    GLenum filter = TextureFilterToOpenGL(m_filter);
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filter));
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filter));
  }

  void Texture2DArray::setBorder(const glm::vec4 & _colour)
  {
    GLCALL(glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_colour)));
  }

  void Texture2DArray::GenerateMipMaps()
  {
    GLCALL(glGenerateMipmap(GL_TEXTURE_2D_ARRAY));
  }

} } // engine::graphics