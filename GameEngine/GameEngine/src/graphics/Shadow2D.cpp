#include "stdafx.h"

#include "Shadow2D.h"

namespace engine {
namespace graphics {

  std::shared_ptr<Shadow2D> Shadow2D::Create(uint _width, uint _height, TextureFormat _format)
  {
    struct enable_shadow : Shadow2D { };

    assert(TextureFormatBase(_format) == TextureBaseFormat::DEPTH_COMPONENT);

    auto shadow = std::make_shared<enable_shadow>();
    shadow->m_width = _width;
    shadow->m_height = _height;

    shadow->m_format = _format;

    GLCALL(
      glTexImage2D(
        GL_TEXTURE_2D, 0,
        TextureFormatToOpenGL(_format),
        _width, _height, 0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
      )
    );

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border));

    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
    GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));

    return shadow;
  }

  Shadow2D::Shadow2D() : Texture(TextureType::SHADOW_2D)
  {
    GLCALL(glGenTextures(1, &m_id));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
  }

  Shadow2D::~Shadow2D()
  {
    GLCALL(glDeleteTextures(1, &m_id));
  }

  void Shadow2D::Bind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
  }

  void Shadow2D::Unbind(uint _unit) const
  {
    GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
    GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
  }

  void Shadow2D::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    GLCALL(
      glTexImage2D(
        GL_TEXTURE_2D, 0,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, 0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
      )
    );
  }

} } // engine::graphics