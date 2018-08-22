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
  
  GLenum TextureFormatToOpenGL(TextureFormat _format)
  {
    return static_cast<GLenum>(_format);
  }
  
  TextureFormat OpenGLToTextureFormat(GLenum _format)
  {
    switch (_format)
    {
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