#include "stdafx.h"

#include "Shadow2D.h"

namespace engine {
namespace graphics {

  Shadow2D::Shadow2D(TextureFormat _format, int _width, int _height) :
    Texture(TextureType::SHADOW_2D)
  {
    if (TextureFormatBase(_format) != TextureBaseFormat::DEPTH_COMPONENT)
    {
      throw std::invalid_argument("invalid texture format");
    }

    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = 1;

    glGenTextures(1, &m_id);
    Bind();

    glTexStorage2D(
      GL_TEXTURE_2D, 1,
      TextureFormatToOpenGL(m_format),
      m_width, m_height
    );

    m_wrap = TextureWrap::CLAMP_TO_BORDER;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    m_filter = TextureFilter::LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  }

  Shadow2D::~Shadow2D()
  {
    glDeleteTextures(1, &m_id);
  }

  void Shadow2D::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, m_id);
  }

  void Shadow2D::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    Bind();
  }

} } // engine::graphics