#ifndef _ENGINE_GRAPHICS_TEXTURETYPES_H_
#define _ENGINE_GRAPHICS_TEXTURETYPES_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class TextureFormat
    {
      RGBA8,
      RGBA16F,
      DEPTH_COMPONENT16,
      DEPTH_COMPONENT24,
      DEPTH_COMPONENT32F,
      DEPTH24_STENCIL8
    };

    GLenum TextureFormatToOpenGL(TextureFormat _format);
    GLenum TextureBaseFormatToOpenGL(TextureFormat _format);

    enum class TextureType
    {
      UNSIGNED_BYTE,
      FLOAT
    };

    GLenum TextureTypeToOpenGL(TextureType _type);

    enum class TextureWrap
    {
      REPEAT,
      MIRROR_REPEAT,
      CLAMP_TO_EDGE,
      CLAMP_TO_BORDER
    };

    GLenum TextureWrapToOpenGL(TextureWrap _wrap);

    enum class TextureFilter
    {
      LINEAR,
      NEAREST
    };

    GLenum TextureFilterToOpenGL(TextureFilter _filter);
  }
}

#endif //_ENGINE_GRAPHICS_TEXTURETYPES_H_