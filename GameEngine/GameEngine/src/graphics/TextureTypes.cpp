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
      default:
      { 
        throw std::invalid_argument("Invalid sampler type");
      }
    }
  }
  
  GLenum TextureBaseFormatToOpenGL(TextureBaseFormat _format)
  {
    return static_cast<GLenum>(_format);
  }
  
  TextureBaseFormat OpenGLToTextureBaseFormat(GLenum _format)
  {
    switch (_format)
    {
      case GL_RGBA:            { return TextureBaseFormat::RGBA;            }
      case GL_DEPTH_COMPONENT: { return TextureBaseFormat::DEPTH_COMPONENT; }
      case GL_DEPTH_STENCIL:   { return TextureBaseFormat::DEPTH_STENCIL;   }
      default: 
      { 
        throw std::invalid_argument("Invalid base texture format");
      }
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
      case GL_RGBA8:              { return TextureFormat::RGBA8;              }
      case GL_RGBA16F:            { return TextureFormat::RGBA16F;            }
      case GL_RGBA32F:            { return TextureFormat::RGBA32F;            }
      case GL_DEPTH_COMPONENT16:  { return TextureFormat::DEPTH_COMPONENT16;  }
      case GL_DEPTH_COMPONENT24:  { return TextureFormat::DEPTH_COMPONENT24;  }
      case GL_DEPTH_COMPONENT32F: { return TextureFormat::DEPTH_COMPONENT32F; }
      case GL_DEPTH24_STENCIL8:   { return TextureFormat::DEPTH24_STENCIL8;   }
      default: 
      {
        throw std::invalid_argument("Invalid texture format");
      }
    }
  }
  
  TextureBaseFormat TextureFormatBase(TextureFormat _format)
  {
    switch (_format)
    {
      case TextureFormat::RGBA8:              { return TextureBaseFormat::RGBA;            }
      case TextureFormat::RGBA16F:            { return TextureBaseFormat::RGBA;            }
      case TextureFormat::DEPTH_COMPONENT16:  { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH_COMPONENT24:  { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH_COMPONENT32F: { return TextureBaseFormat::DEPTH_COMPONENT; }
      case TextureFormat::DEPTH24_STENCIL8:   { return TextureBaseFormat::DEPTH_STENCIL;   }
      default: 
      {
        throw std::invalid_argument("Invalid texture format");
      }
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
  
  GLenum TextureFilterToOpenGL(TextureFilter _filter)
  {
    return static_cast<GLenum>(_filter);
  }
  
  TextureFilter OpenGLToTextureFilter(GLenum _filter)
  {
    switch (_filter)
    {
      case GL_LINEAR: { return TextureFilter::LINEAR; }
      case GL_NEAREST: { return TextureFilter::NEAREST; }
      default: 
      { 
        throw std::invalid_argument("Invalid texture filter.");
      }
    }
  }

} } // engine::graphics