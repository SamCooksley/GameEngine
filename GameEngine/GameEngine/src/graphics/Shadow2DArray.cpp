#include "stdafx.h"

#include "Shadow2DArray.h"

namespace engine {
namespace graphics {

  Shadow2DArray::Shadow2DArray(uint _width, uint _height, uint _count, TextureFormat _format) : 
    Texture(TextureType::SHADOW_2D_ARRAY),
    m_width(_width), m_height(_height), m_count(_count),
    m_format(_format)
  {
    GLCALL(glGenTextures(1, &m_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, m_id));

    GLCALL(
      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY, 1,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, m_count
      )
    );

    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLCALL(glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border));

    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
  }

  Shadow2DArray::~Shadow2DArray()
  {
    GLCALL(glDeleteTextures(1, &m_id));
  }

  void Shadow2DArray::Bind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, m_id));
  }

  void Shadow2DArray::Unbind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
  }

  void Shadow2DArray::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    GLCALL(
      glTexStorage3D(
        GL_TEXTURE_2D_ARRAY, 1,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, m_count
      )
    );
  }

  uint Shadow2DArray::getCount() const
  {
    return m_count;
  }

} } // engine::graphics