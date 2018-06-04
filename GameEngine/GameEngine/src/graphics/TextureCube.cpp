#include "stdafx.h"

#include "TextureCube.h"

#include "utilities\Image.h"

namespace engine {
namespace graphics {

  std::shared_ptr<TextureCube> TextureCube::Load(const std::array<String, 6> & _paths)
  {
    struct enable_texture : public TextureCube { };
  
    std::array<file::ImageData, 6> images;
    for (size_t i = 0; i < _paths.size(); ++i)
    {
      if (!file::LoadImagePowerOf2(_paths[i], &images[i], false))
      {
        throw std::runtime_error("TextureCube Error: failed to load file: " + _paths[i]);
      }
    }
  
    auto cube = std::make_shared<enable_texture>();
    cube->m_width = images[0].width;
    cube->m_height = images[0].height;
    
    cube->m_format = TextureFormat::RGBA8;
    cube->m_dataType = TextureDataType::FLOAT;

    cube->setName(file::getFilenameWithoutExtension(_paths[0]));

    cube->setFilter(TextureFilter::LINEAR);
  
    for (size_t i = 0; i < images.size(); ++i)
    {
      const file::ImageData & image = images[i];
      GLCALL(glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
        GL_RGBA8,
        image.width, image.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE,
        &image.pixels[0]
      ));
    }
  
    return cube;
  }
  
  TextureCube::TextureCube() : 
    Texture(TextureType::TEXTURE_CUBE)
  {
    GLCALL(glGenTextures(1, &m_id));
    GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));
  
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
  }
  
  TextureCube::TextureCube(uint _width, uint _height, TextureFormat _format, TextureDataType _type) :
    TextureCube()
  {
    m_width = _width;
    m_height = _height;

    m_format = _format;
    m_dataType = _type;

    for (size_t i = 0u; i < 6u; ++i)
    {
      GLCALL(glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
          TextureFormatToOpenGL(_format),
          _width, _height, 0,
          TextureBaseFormatToOpenGL(TextureFormatBase(_format)),
          TextureDataTypeToOpenGL(_type),
          nullptr
      ));
    }
  }
  
  TextureCube::TextureCube(uint _width, uint _height, const glm::vec4 & _colour) :
    TextureCube()
  {
    m_width = _width;
    m_height = _height;

    m_format = TextureFormat::RGBA8;
    m_dataType = TextureDataType::FLOAT;

    std::vector<float> pixels(_width * _height * 4);
  
    for (size_t i = 0; i < pixels.size(); i += 4)
    {
      memcpy(&pixels[i], glm::value_ptr(_colour), sizeof(float) * 4u);
    }
  
    for (size_t i = 0; i < 6; ++i)
    {
      GLCALL(glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
        GL_RGBA8,
        _width, _height, 0,
        GL_RGBA, GL_FLOAT,
        &pixels[0]
      ));
    }
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

  void TextureCube::Resize(uint _width, uint _height)
  {
    if (_width == m_width && _height == m_height) { return; }

    m_width = _width;
    m_height = _height;

    for (size_t i = 0u; i < 6u; ++i)
    {
      GLCALL(glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
        TextureFormatToOpenGL(m_format),
        m_width, m_height, 0,
        TextureBaseFormatToOpenGL(TextureFormatBase(m_format)),
        TextureDataTypeToOpenGL(m_dataType),
        nullptr
      ));
    }
  }
  
  void TextureCube::setFilter(TextureFilter _filter)
  {
    m_filter = _filter;
    GLenum filter = TextureFilterToOpenGL(m_filter);
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter));
    GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter));
  }

} } // engine::graphics