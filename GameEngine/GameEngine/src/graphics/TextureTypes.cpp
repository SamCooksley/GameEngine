#include "stdafx.h" 

#include "TextureTypes.h"

namespace engine {
namespace graphics {

  GLenum TextureTypeToOpenGL(TextureType _type)
  {
    switch (_type)
    {
      case TextureType::TEXTURE_2D:       { return GL_TEXTURE_2D;       }
      case TextureType::TEXTURE_2D_ARRAY: { return GL_TEXTURE_2D_ARRAY; }
      case TextureType::TEXTURE_CUBE:     { return GL_TEXTURE_CUBE_MAP; }
      case TextureType::SHADOW_2D:        { return GL_TEXTURE_2D;       }
      case TextureType::SHADOW_2D_ARRAY:  { return GL_TEXTURE_2D_ARRAY; }
      case TextureType::SHADOW_CUBE:      { return GL_TEXTURE_CUBE_MAP; }
      default:
      {
        throw std::invalid_argument("Invalid texture type");
      }
    }
  }
  
  TextureType OpenGLToTextureType(GLenum _type)
  {
    switch (_type)
    {
      case GL_TEXTURE_2D:       { return TextureType::TEXTURE_2D;       }
      case GL_TEXTURE_2D_ARRAY: { return TextureType::TEXTURE_2D_ARRAY; }
      case GL_TEXTURE_CUBE_MAP: { return TextureType::TEXTURE_CUBE;     }
      default: 
      {
        throw std::invalid_argument("Invalid texture type");
      }
    }
  }
  
  GLenum TextureTypeToOpenGLSampler(TextureType _type)
  {
    //map specific values as the enum values are opengl textures, not samplers.
    switch (_type)
    {
      case TextureType::TEXTURE_2D:       { return GL_SAMPLER_2D;              }
      case TextureType::TEXTURE_2D_ARRAY: { return GL_SAMPLER_2D_ARRAY;        }
      case TextureType::TEXTURE_CUBE:     { return GL_SAMPLER_CUBE;            }
      case TextureType::SHADOW_2D:        { return GL_SAMPLER_2D_SHADOW;       }
      case TextureType::SHADOW_2D_ARRAY:  { return GL_SAMPLER_2D_ARRAY_SHADOW; }
      case TextureType::SHADOW_CUBE:      { return GL_SAMPLER_CUBE_SHADOW;     }
      default: 
      { 
        throw std::invalid_argument("Invalid texture type");
      }
    }
  }
  
  TextureType OpenGLToTextureTypeSampler(GLenum _type)
  {
    switch (_type)
    {
      case GL_SAMPLER_2D:              { return TextureType::TEXTURE_2D;       }
      case GL_SAMPLER_2D_ARRAY:        { return TextureType::TEXTURE_2D_ARRAY; }
      case GL_SAMPLER_CUBE:            { return TextureType::TEXTURE_CUBE;     }
      case GL_SAMPLER_2D_SHADOW:       { return TextureType::SHADOW_2D;        }
      case GL_SAMPLER_2D_ARRAY_SHADOW: { return TextureType::SHADOW_2D_ARRAY;  }
      case GL_SAMPLER_CUBE_SHADOW:     { return TextureType::SHADOW_CUBE;      }
      default:
      { 
        throw std::invalid_argument("Invalid sampler type");
      }
    }
  }
  
  GLenum TextureBaseFormatToOpenGL(TextureBaseFormat _format)
  {
    if (_format == TextureBaseFormat::NONE)
    {
      throw std::invalid_argument("invalid base texture format");
    }
    return static_cast<GLenum>(_format);
  }
  
  TextureBaseFormat OpenGLToTextureBaseFormat(GLenum _format)
  {
    switch (_format)
    {
      case GL_RED:             { return TextureBaseFormat::R;               }
      case GL_RG:              { return TextureBaseFormat::RG;              }
      case GL_RGB:             { return TextureBaseFormat::RGB;             }
      case GL_RGBA:            { return TextureBaseFormat::RGBA;            }
      case GL_DEPTH_COMPONENT: { return TextureBaseFormat::DEPTH_COMPONENT; }
      case GL_DEPTH_STENCIL:   { return TextureBaseFormat::DEPTH_STENCIL;   }

      default:                 { return TextureBaseFormat::NONE;            }
    }
  }

  int TextureBaseFormatToChannelCount(TextureBaseFormat _format)
  {
    switch (_format)
    {
      case TextureBaseFormat::R:               { return 1; }
      case TextureBaseFormat::RG:              { return 2; }
      case TextureBaseFormat::RGB:             { return 3; }
      case TextureBaseFormat::RGBA:            { return 4; }
      case TextureBaseFormat::DEPTH_COMPONENT: { return 1; }
      case TextureBaseFormat::DEPTH_STENCIL:   { return 2; } // not sure if correct?
      default:                                 { return 0; }
    }
  }

  TextureBaseFormat ChannelCountToTextureBaseFormat(int _count)
  {
    switch (_count)
    {
      case 1:  { return TextureBaseFormat::R;    }
      case 2:  { return TextureBaseFormat::RG;   }
      case 3:  { return TextureBaseFormat::RGB;  }
      case 4:  { return TextureBaseFormat::RGBA; }
      default: { return TextureBaseFormat::NONE; }
    }
  }
  
  GLenum TextureFormatToOpenGL(TextureFormat _format)
  {
    if (_format == TextureFormat::NONE)
    {
      throw std::invalid_argument("invalid texture format");
    }
    return static_cast<GLenum>(_format);
  }
  
  TextureFormat OpenGLToTextureFormat(GLenum _format)
  {
    switch (_format)
    {
      case GL_R8:                 { return TextureFormat::R8;                 }
      case GL_R16F:               { return TextureFormat::R16F;               }
      case GL_R32F:               { return TextureFormat::R32F;               }

      case GL_RG8:                { return TextureFormat::RG8;                }
      case GL_RG16F:              { return TextureFormat::RG16F;              }
      case GL_RG32F:              { return TextureFormat::RG32F;              }

      case GL_RGB8:               { return TextureFormat::RGB8;               }
      case GL_RGB16F:             { return TextureFormat::RGB16F;             }
      case GL_RGB32F:             { return TextureFormat::RGB32F;             }

      case GL_RGBA8:              { return TextureFormat::RGBA8;              }
      case GL_RGBA16F:            { return TextureFormat::RGBA16F;            }
      case GL_RGBA32F:            { return TextureFormat::RGBA32F;            }

      case GL_SRGB8:              { return TextureFormat::SRGB8;              }
      case GL_SRGB8_ALPHA8:       { return TextureFormat::SRGB_A8;            }

      case GL_DEPTH_COMPONENT16:  { return TextureFormat::DEPTH_COMPONENT16;  }
      case GL_DEPTH_COMPONENT24:  { return TextureFormat::DEPTH_COMPONENT24;  }
      case GL_DEPTH_COMPONENT32:  { return TextureFormat::DEPTH_COMPONENT32;  }
      case GL_DEPTH_COMPONENT32F: { return TextureFormat::DEPTH_COMPONENT32F; }
      case GL_DEPTH24_STENCIL8:   { return TextureFormat::DEPTH24_STENCIL8;   }

      default:                    { return TextureFormat::NONE;               }
    }
  }
  
  TextureBaseFormat TextureFormatBase(TextureFormat _format)
  {
    switch (_format)
    {
      case TextureFormat::R8:                 { return TextureBaseFormat::R;               }
      case TextureFormat::R16F:               { return TextureBaseFormat::R;               }
      case TextureFormat::R32F:               { return TextureBaseFormat::R;               }

      case TextureFormat::RG8:                { return TextureBaseFormat::RG;              }
      case TextureFormat::RG16F:              { return TextureBaseFormat::RG;              }
      case TextureFormat::RG32F:              { return TextureBaseFormat::RG;              }

      case TextureFormat::RGB8:               { return TextureBaseFormat::RGB;             }
      case TextureFormat::RGB16F:             { return TextureBaseFormat::RGB;             }
      case TextureFormat::RGB32F:             { return TextureBaseFormat::RGB;             }

      case TextureFormat::RGBA8:              { return TextureBaseFormat::RGBA;            }
      case TextureFormat::RGBA16F:            { return TextureBaseFormat::RGBA;            }
      case TextureFormat::RGBA32F:            { return TextureBaseFormat::RGBA;            }

      case TextureFormat::SRGB8:              { return TextureBaseFormat::RGB;             }
      case TextureFormat::SRGB_A8:            { return TextureBaseFormat::RGBA;            }

      case TextureFormat::DEPTH_COMPONENT16:  { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH_COMPONENT24:  { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH_COMPONENT32:  { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH_COMPONENT32F: { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH24_STENCIL8:   { return TextureBaseFormat::DEPTH_STENCIL;   }

      default:                                { return TextureBaseFormat::NONE;            }
    }
  }

  TextureFormat CreateTextureFormat(TextureBaseFormat _base, int _size, bool _float)
  {
    switch (_base)
    {
      case TextureBaseFormat::R:
      {
        switch (_size)
        {
          case 8:  { return TextureFormat::R8;   }
          case 16: { return TextureFormat::R16F; }
          case 32: { return TextureFormat::R32F; }
        }
        break;
      }
      case TextureBaseFormat::RG:
      {
        switch (_size)
        {
          case 8:  { return TextureFormat::RG8;   }
          case 16: { return TextureFormat::RG16F; }
          case 32: { return TextureFormat::RG32F; }
        }
        break;
      }
      case TextureBaseFormat::RGB:
      {
        switch (_size)
        {
          case 8:  { return TextureFormat::RGB8;   }
          case 16: { return TextureFormat::RGB16F; }
          case 32: { return TextureFormat::RGB32F; }
        }
        break;
      }
      case TextureBaseFormat::RGBA:
      {
        switch (_size)
        {
          case 8:  { return TextureFormat::RGBA8;   }
          case 16: { return TextureFormat::RGBA16F; }
          case 32: { return TextureFormat::RGBA32F; }
        }
        break;
      }
      case TextureBaseFormat::DEPTH_COMPONENT:
      {
        switch (_size)
        {
          case 16: { return TextureFormat::DEPTH_COMPONENT16; }
          case 24: { return TextureFormat::DEPTH_COMPONENT24; }
          case 32: { return _float ? TextureFormat::DEPTH_COMPONENT32F : TextureFormat::DEPTH_COMPONENT32; }
        }
        break;
      }
    }

    return TextureFormat::NONE;
  }
  
  GLenum TextureDataTypeToOpenGL(TextureDataType _type)
  {
    return static_cast<GLenum>(_type);
  }
  
  TextureDataType OpenGLToTextureDataType(GLenum _type)
  {
    switch (_type)
    {
      case GL_UNSIGNED_BYTE: { return TextureDataType::UNSIGNED_BYTE; }
      case GL_FLOAT:         { return TextureDataType::FLOAT;         }
      default: 
      {
        throw std::invalid_argument("Invalid texture data type");
      }
    }
  }
  
  GLenum TextureWrapToOpenGL(TextureWrap _wrap)
  {
    return static_cast<GLenum>(_wrap);
  }
  
  TextureWrap OpenGLToTextureWrap(GLenum _wrap)
  {
    switch (_wrap)
    {
      case GL_REPEAT:          { return TextureWrap::REPEAT;          }
      case GL_MIRRORED_REPEAT: { return TextureWrap::MIRROR_REPEAT;   }
      case GL_CLAMP_TO_EDGE:   { return TextureWrap::CLAMP_TO_EDGE;   }
      case GL_CLAMP_TO_BORDER: { return TextureWrap::CLAMP_TO_BORDER; }
      default: 
      { 
        throw std::invalid_argument("Invalid texture wrap");
      }
    }
  }
  
  GLenum TextureFilterToOpenGL(TextureFilter _filter, bool _mipmaps)
  {
    switch (_filter)
    {
      case TextureFilter::LINEAR:  { return _mipmaps ? GL_LINEAR_MIPMAP_LINEAR   : GL_LINEAR;  }
      case TextureFilter::NEAREST: { return _mipmaps ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST; }
      default: { assert(false); }
    }
  }
  
  TextureFilter OpenGLToTextureFilter(GLenum _filter)
  {
    switch (_filter)
    {
      case GL_LINEAR:                 { return TextureFilter::LINEAR;  }
      case GL_LINEAR_MIPMAP_LINEAR:   { return TextureFilter::LINEAR;  }
      case GL_NEAREST:                { return TextureFilter::NEAREST; }
      case GL_NEAREST_MIPMAP_NEAREST: { return TextureFilter::NEAREST; }
      default: 
      { 
        throw std::invalid_argument("Invalid texture filter.");
      }
    }
  }

} } // engine::graphics