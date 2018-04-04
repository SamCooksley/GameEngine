#include "stdafx.h"

#include "Texture2D.h"

#include "utilities\Image.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Texture2D> Texture2D::Create()
    {
      struct enable_texture : public Texture2D { };

      return std::make_shared<enable_texture>();
    }

    std::shared_ptr<Texture2D> Texture2D::Create(uint _width, uint _height, const glm::vec4 & _colour)
    {
      std::vector<float> pixels(_width * _height * 4u);

      for (size_t i = 0; i < pixels.size(); i += 4)
      {
        memcpy(&pixels[i], glm::value_ptr(_colour), sizeof(float) * 4u);
      }

      auto texture = Create();
      texture->m_width = _width;
      texture->m_height = _height;

      GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, &pixels[0]));

      texture->setWrap(TextureWrap::CLAMP_TO_EDGE);
      texture->setFilter(TextureFilter::NEAREST);

      return texture;
    }

    std::shared_ptr<Texture2D> Texture2D::Load(const std::string & _path, bool _mipmaps)
    {
      file::ImageData image;
      if (!file::LoadImagePowerOf2(_path, &image, false))
      {
        throw std::runtime_error("Texture2D Error: failed to load file: " + _path);
      }

      auto texture = Create();
      texture->setName(file::getFilenameWithoutExtension(_path));
      texture->m_width = image.width;
      texture->m_height = image.height;

      GLCALL(glTexImage2D(
        GL_TEXTURE_2D, 0, 
        GL_RGBA, 
        image.width, image.height, 
        0,
        GL_RGBA, GL_UNSIGNED_BYTE, 
        &image.pixels[0]
      ));

      texture->setWrap(TextureWrap::REPEAT);
      texture->setFilter(TextureFilter::LINEAR);

      if (_mipmaps)
      {
        texture->GenerateMipMaps();
      }

      return texture;
    }

    Texture2D::Texture2D()
    {
      GLCALL(glGenTextures(1, &m_id));
      GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    Texture2D::~Texture2D()
    {
      GLCALL(glDeleteTextures(1, &m_id));
    }

    void Texture2D::Bind(GLenum _unit) const
    {
      GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
      GLCALL(glBindTexture(GL_TEXTURE_2D, m_id));
    }

    void Texture2D::Unbind(GLenum _unit) const
    {
      GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
      GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture2D::setWrap(TextureWrap _wrap)
    {
      m_wrap = _wrap;
      GLenum wrap = TextureWrapToOpenGL(m_wrap);
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap));
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap));
    }

    void Texture2D::setFilter(TextureFilter _filter)
    {
      m_filter = _filter;
      GLenum filter = TextureFilterToOpenGL(m_filter);
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter));
    }

    void Texture2D::setBorder(const glm::vec4 & _colour)
    {
      GLCALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_colour)));
    }

    void Texture2D::GenerateMipMaps()
    {
      GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
      GLenum filter = m_filter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST;
      GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter));
    }
  }
}