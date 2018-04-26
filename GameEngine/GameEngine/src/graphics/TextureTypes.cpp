#include "stdafx.h" 

#include "TextureTypes.h"

namespace engine
{
  namespace graphics
  {
    GLenum SamplerTypeToOpenGL(SamplerType _sampler)
    {
      return (GLenum)_sampler;
    }

    SamplerType OpenGLToSamplerType(GLenum _sampler)
    {
      switch (_sampler)
      {
      case GL_SAMPLER_2D:   { return SamplerType::SAMPLER_2D;   }
      case GL_SAMPLER_CUBE: { return SamplerType::SAMPLER_CUBE; }
      default: { assert(false); }
      }

      return SamplerType::SAMPLER_2D;
    }

    GLenum TextureBaseFormatToOpenGL(TextureBaseFormat _format)
    {
      return (GLenum)_format;
    }

    TextureBaseFormat OpenGLToTextureBaseFormat(GLenum _format)
    {
      switch (_format)
      {
        case GL_RGBA:            { return TextureBaseFormat::RGBA;            }
        case GL_DEPTH_COMPONENT: { return TextureBaseFormat::DEPTH_COMPONENT; }
        case GL_DEPTH_STENCIL:   { return TextureBaseFormat::DEPTH_STENCIL;   }
        default: { assert(false); }
      }

      return TextureBaseFormat::RGBA;
    }

    GLenum TextureFormatToOpenGL(TextureFormat _format)
    {
      return (GLenum)_format;
    }

    TextureFormat OpenGLToTextureFormat(GLenum _format)
    {
      switch (_format)
      {
        case GL_RGBA8:              { return TextureFormat::RGBA8;              }
        case GL_RGBA16F:            { return TextureFormat::RGBA16F;            }
        case GL_DEPTH_COMPONENT16:  { return TextureFormat::DEPTH_COMPONENT16;  }
        case GL_DEPTH_COMPONENT24:  { return TextureFormat::DEPTH_COMPONENT24;  }
        case GL_DEPTH_COMPONENT32F: { return TextureFormat::DEPTH_COMPONENT32F; }
        case GL_DEPTH24_STENCIL8:   { return TextureFormat::DEPTH24_STENCIL8;   }
        default: { assert(false); }
      }

      return TextureFormat::RGBA8;
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
        default: { assert(false); }
      }

      return TextureBaseFormat::RGBA;
    }

    GLenum TextureTypeToOpenGL(TextureType _type)
    {
      return (GLenum)_type;
    }

    TextureType OpenGLToTextureType(GLenum _type)
    {
      switch (_type)
      {
        case GL_UNSIGNED_BYTE: { return TextureType::UNSIGNED_BYTE; }
        case GL_FLOAT:         { return TextureType::FLOAT;         }
        default: { assert(false); }
      }

      return TextureType::UNSIGNED_BYTE;
    }

    GLenum TextureWrapToOpenGL(TextureWrap _wrap)
    {
      return (GLenum)_wrap;
    }

    TextureWrap OpenGLToTextureWrap(GLenum _wrap)
    {
      switch (_wrap)
      {
        case GL_REPEAT:          { return TextureWrap::REPEAT;          }
        case GL_MIRRORED_REPEAT: { return TextureWrap::MIRROR_REPEAT;   }
        case GL_CLAMP_TO_EDGE:   { return TextureWrap::CLAMP_TO_EDGE;   }
        case GL_CLAMP_TO_BORDER: { return TextureWrap::CLAMP_TO_BORDER; }
        default: { assert(false); }
      }

      return TextureWrap::REPEAT;
    }

    GLenum TextureFilterToOpenGL(TextureFilter _filter)
    {
      return (GLenum)_filter;
    }

    TextureFilter OpenGLToTextureFilter(GLenum _filter)
    {
      switch (_filter)
      {
        case GL_LINEAR: { return TextureFilter::LINEAR; }
        case GL_NEAREST: { return TextureFilter::NEAREST; }
        default: { assert(false); }
      }

      return TextureFilter::LINEAR;
    }
  }
}