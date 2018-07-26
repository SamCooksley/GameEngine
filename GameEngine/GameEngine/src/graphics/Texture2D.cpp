#include "stdafx.h"

#include "Texture2D.h"

#include "utilities\Image.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  std::shared_ptr<Texture2D> Texture2D::Load(const String & _path, int _mipmaps)
  {
    file::ImageData image;
    if (!file::LoadImagePowerOf2(_path, &image, false))
    {
      throw std::runtime_error("Texture2D Error: failed to load file: " + _path);
    }
  
    auto texture = std::make_shared<Texture2D>(
      TextureFormat::RGBA8, 
      image.width, image.height,
      _mipmaps
    );
    texture->setName(file::getFilenameWithoutExtension(_path));
  
    glTexSubImage2D(
      GL_TEXTURE_2D, 0,
      0, 0,
      image.width, image.height, 
      GL_RGBA, GL_UNSIGNED_BYTE, &image.pixels[0]
    );

    texture->GenerateMipMaps();

    texture->setWrap(TextureWrap::REPEAT);
    texture->setFilter(TextureFilter::LINEAR);
    // TODO:
    texture->setAnisotropy(8.f);

    return texture;
  }
  
  Texture2D::Texture2D(TextureFormat _format, int _width, int _height, int _mipmaps) :
    Texture(TextureType::TEXTURE_2D)
  {
    m_format = _format;
    m_width = _width;
    m_height = _height;

    m_mipmaps = _mipmaps;
    if (m_mipmaps <= 0)
    {
      m_mipmaps = CalculateMipMapCount(_width, _height);
    }

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexStorage2D(GL_TEXTURE_2D, m_mipmaps, TextureFormatToOpenGL(m_format), m_width, m_height);

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::LINEAR);
  }

  Texture2D::Texture2D(int _width, int _height, const glm::vec4 & _colour) :
    Texture2D(TextureFormat::RGBA8, _width, _height, 1)
  {
    std::vector<float> pixels(m_width * m_height * 4);

    for (size_t i = 0; i < pixels.size(); i += 4)
    {
      memcpy(&pixels[i], glm::value_ptr(_colour), sizeof(float) * 4u);
    }

    glTexSubImage2D(
      GL_TEXTURE_2D, 0,
      0, 0,
      _width, _height,
      GL_RGBA, GL_FLOAT, &pixels[0]
    );

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::NEAREST);
  }
  
  Texture2D::~Texture2D()
  {
    glDeleteTextures(1, &m_id);
  }
  
  void Texture2D::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
  }
  
  void Texture2D::setWrap(TextureWrap _wrap)
  {
    m_wrap = _wrap;
    GLenum wrap = TextureWrapToOpenGL(m_wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  }
  
  void Texture2D::setFilter(TextureFilter _filter)
  {
    m_filter = _filter;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(m_filter, true));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(m_filter, false));
  }
  
  void Texture2D::setBorder(const glm::vec4 & _colour)
  {
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_colour));
  }

  void Texture2D::setAnisotropy(float _anisotropy)
  {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, glm::min(_anisotropy, Graphics::GL().getMaxAnisotropy()));
  }

  void Texture2D::GenerateMipMaps()
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

} } // engine::graphics