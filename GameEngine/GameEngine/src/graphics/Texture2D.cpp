#include "stdafx.h"

#include "Texture2D.h"

#include "Texture2DArray.h"

#include "utilities\Image.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  std::shared_ptr<Texture2D> Texture2D::Load(const String & _path, bool _linear, int _mipmaps)
  {
    file::ImageData image;
    if (!file::LoadImagePowerOf2(_path, &image, false))
    {
      return nullptr;
    }
  
    auto texture = std::make_shared<Texture2D>(
      _linear ? TextureFormat::RGBA8 : TextureFormat::SRGB_A8, 
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

  std::unique_ptr<Texture2D> Texture2D::CloneFormat(const Texture2D & _src)
  {
    auto texture = std::make_unique<Texture2D>(_src.m_format, _src.m_width, _src.m_height, _src.m_mipmaps);
    texture->setWrap(_src.m_wrap);
    texture->setFilter(_src.m_filter);

    return texture;
  }

  bool Texture2D::CompareFormat(const Texture2D & _lhs, const Texture2D & _rhs)
  {
    return _lhs.m_format  == _rhs.m_format 
        && _lhs.m_width   == _rhs.m_width 
        && _lhs.m_height  == _rhs.m_height
        && _lhs.m_mipmaps == _rhs.m_mipmaps;
  }
  
  Texture2D::Texture2D(TextureFormat _format, int _width, int _height, int _mipmaps) :
    Texture(TextureType::TEXTURE_2D)
  {
    m_format = _format;
    m_width = _width;
    m_height = _height;
    m_depth = 1;

    m_mipmaps = _mipmaps;
    if (m_mipmaps <= 0)
    {
      m_mipmaps = CalculateMipMapCount(_width, _height);
    }

    glGenTextures(1, &m_id);
    Bind();

    glTexStorage2D(GL_TEXTURE_2D, m_mipmaps, TextureFormatToOpenGL(m_format), m_width, m_height);

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::LINEAR);
  }

  Texture2D::Texture2D(int _width, int _height, const glm::vec4 & _colour) :
    Texture2D(
      CreateTextureFormat(ChannelCountToTextureBaseFormat(_colour.length()), 8),
      _width, _height, 1
    )
  {
    using value_type = glm::vec4::value_type;
    auto count = _colour.length();
    TextureBaseFormat format = ChannelCountToTextureBaseFormat(count);

    std::vector<value_type> pixels(m_width * m_height * count);

    for (size_t i = 0; i < pixels.size(); i += count)
    {
      memcpy(&pixels[i], glm::value_ptr(_colour), sizeof(value_type) * count);
    }

    glTexSubImage2D(
      GL_TEXTURE_2D, 0,
      0, 0,
      _width, _height,
      TextureBaseFormatToOpenGL(format),
      TypeToOpenGL(TypeData<value_type>::type),
      &pixels[0]
    );

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::NEAREST);
  }

  Texture2D::Texture2D(const Texture2D & _src, int _level, int _levelCount) :
    Texture(TextureType::TEXTURE_2D)
  {
    m_format = _src.m_format;
    m_width = _src.getMipMapWidth(_level);
    m_height = _src.getMipMapHeight(_level);
    m_depth = 1;

    glGenTextures(1, &m_id);

    glTextureView(
      m_id,
      GL_TEXTURE_2D,
      _src.m_id,
      TextureFormatToOpenGL(m_format),
      _level, _levelCount,
      0, 1
    );

    Bind();

    setWrap(TextureWrap::REPEAT);
    setFilter(TextureFilter::NEAREST);
  }

  Texture2D::Texture2D(const Texture2DArray & _src, int _depth, int _level, int _levelCount) :
    Texture(TextureType::TEXTURE_2D)
  {
    m_format = _src.m_format;
    m_width = _src.getMipMapWidth(_level);
    m_height = _src.getMipMapHeight(_level);
    m_depth = 1;

    glGenTextures(1, &m_id);

    glTextureView(
      m_id,
      GL_TEXTURE_2D,
      _src.m_id,
      TextureFormatToOpenGL(m_format),
      _level, _levelCount,
      _depth, 1
    );

    Bind();

    setWrap(_src.m_wrap);
    setFilter(_src.m_filter);
  }
  
  Texture2D::~Texture2D()
  {
    glDeleteTextures(1, &m_id);
  }
  
  void Texture2D::Bind() const
  {
    glBindTexture(GL_TEXTURE_2D, m_id);
  }

  void Texture2D::Bind(int _unit) const
  {
    glActiveTexture(GL_TEXTURE0 + _unit);
    Bind();
  }

  void Texture2D::setData(TextureBaseFormat _format, TextureDataType _type, const void * _data, size_t _count)
  {
    int channels = TextureBaseFormatToChannelCount(_format);
    int pixelCount = _count / channels;

    if (pixelCount != m_width * m_height)
    {
      throw std::invalid_argument("pixel data size not matching texture size");
    }

    glTexSubImage2D(
      GL_TEXTURE_2D, 0,
      0, 0,
      m_width, m_height,
      TextureBaseFormatToOpenGL(_format),
      TextureDataTypeToOpenGL(_type),
      _data
    );
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