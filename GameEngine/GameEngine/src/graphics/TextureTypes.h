#ifndef _ENGINE_GRAPHICS_TEXTURETYPES_H_
#define _ENGINE_GRAPHICS_TEXTURETYPES_H_

#include "TypeData.h"

namespace engine {
namespace graphics {

  enum class TextureType : GLenum
  {
    NONE,
    TEXTURE_2D,
    TEXTURE_2D_ARRAY,
    TEXTURE_CUBE,
    SHADOW_2D,
    SHADOW_2D_ARRAY,
    SHADOW_CUBE
  };
  
  GLenum TextureTypeToOpenGL(TextureType _type);
  TextureType OpenGLToTextureType(GLenum _type); 
  
  GLenum TextureTypeToOpenGLSampler(TextureType _type);
  TextureType OpenGLToTextureTypeSampler(GLenum _type);
  
  enum class TextureBaseFormat : GLenum
  {
    NONE,
    R = GL_RED,
    RG = GL_RG,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
    DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
    DEPTH_STENCIL = GL_DEPTH_STENCIL
  };
  
  GLenum TextureBaseFormatToOpenGL(TextureBaseFormat _format);
  TextureBaseFormat OpenGLToTextureBaseFormat(GLenum _format);

  TextureBaseFormat ChannelCountToTextureBaseFormat(int _count);
  
  enum class TextureFormat : GLenum
  {
    NONE,

    R8 = GL_R8,
    R16F = GL_R16F,
    R32F = GL_R32F,

    RG8 = GL_RG8,
    RG16F = GL_RG16F,
    RG32F = GL_RG32F,

    RGB8 = GL_RGB8,
    RGB16F = GL_RGB16F,
    RGB32F = GL_RGB32F,

    RGBA8 = GL_RGBA8,
    RGBA16F = GL_RGBA16F,
    RGBA32F = GL_RGBA32F,

    SRGB8 = GL_SRGB8,
    SRGB_A8 = GL_SRGB8_ALPHA8,

    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
  };
  
  GLenum TextureFormatToOpenGL(TextureFormat _format);
  TextureFormat OpenGLToTextureFormat(GLenum _format);
  
  TextureFormat CreateTextureFormat(TextureBaseFormat _base, int _size, bool _float = false);

  TextureBaseFormat TextureFormatBase(TextureFormat _format);
  
  enum class TextureDataType : GLenum
  {
    UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
    FLOAT = GL_FLOAT
  };
  
  GLenum TextureDataTypeToOpenGL(TextureDataType _type);
  TextureDataType OpenGLToTextureDataType(GLenum _type);
  
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
    LINEAR,
    NEAREST
  };
  
  GLenum TextureFilterToOpenGL(TextureFilter _filter, bool _mipmaps);
  TextureFilter OpenGLToTextureFilter(GLenum _filter);

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURETYPES_H_