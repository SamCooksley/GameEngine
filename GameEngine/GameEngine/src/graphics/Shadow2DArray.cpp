#include "stdafx.h"

#include "Shadow2DArray.h"

namespace engine {
namespace graphics {

  Shadow2DArray::Shadow2DArray(TextureFormat _format, uint _width, uint _height, uint _count) :
    Texture(TextureType::SHADOW_2D_ARRAY)
  {
    if (TextureFormatBase(_format) != TextureBaseFormat::DEPTH_COMPONENT)
    {
      throw std::invalid_argument("invalid texture format");
    }

    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = _count;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

    glTexStorage3D(
      GL_TEXTURE_2D_ARRAY, 1,
      TextureFormatToOpenGL(m_format),
      m_width, m_height, m_depth
    );

    m_wrap = TextureWrap::CLAMP_TO_BORDER;
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    m_filter = TextureFilter::LINEAR;
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, border);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  }

  Shadow2DArray::~Shadow2DArray()
  {
    glDeleteTextures(1, &m_id);
  }

  void Shadow2DArray::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
  }

} } // engine::graphics