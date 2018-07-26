#include "stdafx.h"

#include "ShadowCube.h"

namespace engine {
namespace graphics {

  ShadowCube::ShadowCube(TextureFormat _format, int _width, int _height) :
    Texture(TextureType::SHADOW_CUBE)
  {
    if (TextureFormatBase(_format) != TextureBaseFormat::DEPTH_COMPONENT)
    {
      throw std::invalid_argument("invalid texture format");
    }

    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = 6;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

    glTexStorage2D(
      GL_TEXTURE_CUBE_MAP, 1,
      TextureFormatToOpenGL(m_format),
      m_width, m_height
    );

    m_wrap = TextureWrap::CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_filter = TextureFilter::LINEAR;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  }

  ShadowCube::~ShadowCube()
  {
    glDeleteTextures(1, &m_id);
  }

  void ShadowCube::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
  }

} } // engine::graphics