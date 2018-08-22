#include "stdafx.h"

#include "TextureCube.h"

#include "utilities\Image.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  std::shared_ptr<TextureCube> TextureCube::Load(const std::array<String, 6> & _paths, bool _linear, int _mipmaps)
  {
    std::array<file::ImageData, 6> images;
    for (size_t i = 0; i < _paths.size(); ++i)
    {
      if (!file::LoadImagePowerOf2(_paths[i], &images[i], false))
      {
        throw std::runtime_error("TextureCube Error: failed to load file: " + _paths[i]);
      }
    }
  
    auto cube = std::make_shared<TextureCube>(
      _linear ? TextureFormat::RGBA8 : TextureFormat::SRGB_A8,
      images[0].width, images[0].height, _mipmaps
    );
    cube->setName(file::getFilenameWithoutExtension(_paths[0]));
  
    for (size_t i = 0; i < 6; ++i)
    {
      const file::ImageData & image = images[i];
      glTexSubImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
        0, 0, images[0].width, images[0].height,
        GL_RGBA, GL_UNSIGNED_BYTE,
        &images[i].pixels[0]
      );
    }

    cube->GenerateMipMaps();
  
    return cube;
  }
  
  TextureCube::TextureCube(TextureFormat _format, int _width, int _height, int _mipmaps) :
    Texture(TextureType::TEXTURE_CUBE)
  {
    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = 6;

    m_mipmaps = _mipmaps;
    if (m_mipmaps <= 0)
    {
      m_mipmaps = CalculateMipMapCount(_width, _height);
    }

    glGenTextures(1, &m_id);
    Bind();

    glTexStorage2D(
      GL_TEXTURE_CUBE_MAP, m_mipmaps,
      TextureFormatToOpenGL(m_format),
      m_width, m_height
    );

    m_wrap = TextureWrap::CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    setFilter(TextureFilter::LINEAR);
  }

  TextureCube::TextureCube(const TextureCube & _src, int _level, int _levelCount) :
    Texture(TextureType::TEXTURE_CUBE)
  {
    m_format = _src.m_format;
    m_width = _src.getMipMapWidth(_level);
    m_height = _src.getMipMapHeight(_level);
    m_depth = 6;

    glGenTextures(1, &m_id);

    glTextureView(
      m_id, 
      GL_TEXTURE_CUBE_MAP,
      _src.m_id, 
      TextureFormatToOpenGL(m_format), 
      _level, _levelCount,
      0, 6
    );

    Bind();

    m_wrap = TextureWrap::CLAMP_TO_EDGE;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    setFilter(_src.m_filter);
  }

  TextureCube::~TextureCube()
  {
    glDeleteTextures(1, &m_id);
  }
  
  void TextureCube::Bind() const
  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
  }
  
  void TextureCube::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    Bind();
  }
  
  void TextureCube::setFilter(TextureFilter _filter)
  {
    m_filter = _filter;
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(m_filter, true));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(m_filter, false));
  }

  void TextureCube::setAnisotropy(float _anisotropy)
  {
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY, glm::min(_anisotropy, Graphics::GL().getMaxAnisotropy()));
  }

  void TextureCube::GenerateMipMaps()
  {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  }

} } // engine::graphics