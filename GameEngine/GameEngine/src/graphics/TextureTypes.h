#ifndef _ENGINE_GRAPHICS_TEXTURETYPES_H_
#define _ENGINE_GRAPHICS_TEXTURETYPES_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class TextureBaseFormat : GLenum
    {
      RGBA = GL_RGBA,
      DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
      DEPTH_STENCIL = GL_DEPTH_STENCIL
    };

    GLenum TextureBaseFormatToOpenGL(TextureBaseFormat _format);
    TextureBaseFormat OpenGLToTextureBaseFormat(GLenum _format);

    enum class TextureFormat : GLenum
    {
      RGBA8 = GL_RGBA8,
      RGBA16F = GL_RGBA16F,
      DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
      DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
      DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
      DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
    };

    GLenum TextureFormatToOpenGL(TextureFormat _format);
    TextureFormat OpenGLToTextureFormat(GLenum _format);

    TextureBaseFormat TextureBaseFormatToOpenGL(TextureFormat _format);

    enum class TextureType : GLenum
    {
      UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
      FLOAT = GL_FLOAT
    };

    GLenum TextureTypeToOpenGL(TextureType _type);
    TextureType OpenGLToTextureType(GLenum _type);

    enum class TextureWrap : GLenum
    {
      REPEAT = GL_REPEAT,
      MIRROR_REPEAT = GL_MIRRORED_REPEAT,
      CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
      CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    GLenum TextureWrapToOpenGL(TextureWrap _wrap);
    TextureWrap OpenGLToTextureWrap(GLenum _wrap);

    enum class TextureFilter : GLenum
    {
      LINEAR = GL_LINEAR,
      NEAREST = GL_NEAREST
    };

    GLenum TextureFilterToOpenGL(TextureFilter _filter);
    TextureFilter OpenGLToTextureFilter(GLenum _filter);
  }
}

#endif //_ENGINE_GRAPHICS_TEXTURETYPES_H_