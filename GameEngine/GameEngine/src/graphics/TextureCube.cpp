#include "stdafx.h"

#include "TextureCube.h"

#include "utilities\Image.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<TextureCube> TextureCube::Create()
    {
      struct enable_texture : public TextureCube { };

      return std::make_shared<enable_texture>();
    }

    std::shared_ptr<TextureCube> TextureCube::Load(const std::array<std::string, 6> & _paths)
    {
      std::array<file::ImageData, 6> images;
      for (size_t i = 0; i < _paths.size(); ++i)
      {
        if (!file::LoadImagePowerOf2(_paths[i], &images[i], false))
        {
          throw std::runtime_error("TextureCube Error: failed to load file: " + _paths[i]);
        }
      }

      auto cube = Create();
      cube->m_width = images[0].width;
      cube->m_height = images[0].height;
      cube->setName(file::getFilenameWithoutExtension(_paths[0]));
      cube->setFilter(TextureFilter::LINEAR);

      for (size_t i = 0; i < images.size(); ++i)
      {
        const file::ImageData & image = images[i];
        GLCALL(glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
          GL_RGBA,
          image.width, image.height,
          0, GL_RGBA, GL_UNSIGNED_BYTE,
          &image.pixels[0]
        ));
      }

      return cube;
    }

    std::shared_ptr<TextureCube> TextureCube::Create(uint _width, uint _height, const glm::vec4 & _colour)
    {
      std::vector<float> pixels(_width * _height * 4);

      for (size_t i = 0; i < pixels.size(); i += 4)
      {
        memcpy(&pixels[i], glm::value_ptr(_colour), sizeof(float) * 4u);
      }

      auto cube = Create();
      cube->setFilter(TextureFilter::LINEAR);

      for (size_t i = 0; i < 6; ++i)
      {
        GLCALL(glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
          GL_RGBA,
          _width, _height, 0,
          GL_RGBA, GL_FLOAT,
          &pixels[0]
        ));
      }

      return cube;
    }

    TextureCube::TextureCube()
    {
      GLCALL(glGenTextures(1, &m_id));
      GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

      GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
      GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
      GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    }

    TextureCube::~TextureCube()
    {
      GLCALL(glDeleteTextures(1, &m_id));
    }

    void TextureCube::Bind(GLenum _unit) const
    {
      GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
      GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));
    }

    void TextureCube::Unbind(GLenum _unit) const
    {
      GLCALL(glActiveTexture(GL_TEXTURE0 + _unit));
      GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void TextureCube::setFilter(TextureFilter _filter)
    {
      m_filter = _filter;
      GLenum filter = TextureFilterToOpenGL(m_filter);
      GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter));
      GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter));
    }
  }
}