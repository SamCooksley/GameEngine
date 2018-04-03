#include "stdafx.h" 

#include "TextureTypes.h"

namespace engine
{
  namespace graphics
  {
    GLenum TextureFormatToOpenGL(TextureFormat _format)
    {
      switch (_format)
      {
        case TextureFormat::RGBA8:              { return GL_RGBA8;              }
        case TextureFormat::RGBA16F:            { return GL_RGBA16F;            }
        case TextureFormat::DEPTH_COMPONENT16:  { return GL_DEPTH_COMPONENT16;  }
        case TextureFormat::DEPTH_COMPONENT24:  { return GL_DEPTH_COMPONENT24;  }
        case TextureFormat::DEPTH_COMPONENT32F: { return GL_DEPTH_COMPONENT32F; }
        case TextureFormat::DEPTH24_STENCIL8:   { return GL_DEPTH24_STENCIL8;   }
        default: { return 0; }
      }
    }

    GLenum TextureBaseFormatToOpenGL(TextureFormat _format)
    {
      switch (_format)
      {
        case TextureFormat::RGBA8:              { return GL_RGBA;            }
        case TextureFormat::RGBA16F:            { return GL_RGBA;            }
        case TextureFormat::DEPTH_COMPONENT16:  { return GL_DEPTH_COMPONENT; }
        case TextureFormat::DEPTH_COMPONENT24:  { return GL_DEPTH_COMPONENT; }
        case TextureFormat::DEPTH_COMPONENT32F: { return GL_DEPTH_COMPONENT; }
        case TextureFormat::DEPTH24_STENCIL8:   { return GL_DEPTH_STENCIL;   }
        default: { return 0; }
      }
    }

    GLenum TextureTypeToOpenGL(TextureType _type)
    {
      switch (_type)
      {
        case TextureType::UNSIGNED_BYTE: { return GL_UNSIGNED_BYTE; }
        case TextureType::FLOAT:         { return GL_FLOAT;         }
        default: { return 0; }
      }
    }

    GLenum TextureWrapToOpenGL(TextureWrap _wrap)
    {
      switch (_wrap) 
      {
        case TextureWrap::REPEAT:          { return GL_REPEAT;          }
        case TextureWrap::MIRROR_REPEAT:   { return GL_MIRRORED_REPEAT; }
        case TextureWrap::CLAMP_TO_EDGE:   { return GL_CLAMP_TO_EDGE;   }
        case TextureWrap::CLAMP_TO_BORDER: { return GL_CLAMP_TO_BORDER; }
        default: { return 0; }
      }
    }

    GLenum TextureFilterToOpenGL(TextureFilter _filter)
    {
      switch (_filter)
      {
        case TextureFilter::LINEAR:  { return GL_LINEAR;  }
        case TextureFilter::NEAREST: { return GL_NEAREST; }
        default: { return 0; }
      }
    }
  }
}