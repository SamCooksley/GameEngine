#ifndef _ENGINE_GRAPHICS_TEXTURETYPES_H_
#define _ENGINE_GRAPHICS_TEXTURETYPES_H_

#include "TypeData.h"

namespace engine {
namespace graphics {

  enum class TextureType : GLenum
  {
    TEXTURE_2D,
    TEXTURE_2D_ARRAY,
    TEXTURE_CUBE,
    SHADOW_2D,
    SHADOW_2D_ARRAY
  };
  
  GLenum TextureTypeToOpenGL(TextureType _type);
  TextureType OpenGLToTextureType(GLenum _type); 
  
  GLenum TextureTypeToOpenGLSampler(TextureType _type);
  TextureType OpenGLToTextureTypeSampler(GLenum _type);
  
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
    RG32F = GL_RG32F,
    RGBA8 = GL_RGBA8,
    RGBA16F = GL_RGBA16F,
    RGBA32F = GL_RGBA32F,
    DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
    DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT32 = GL_DEPTH_COMPONENT32,
    DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
    DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
  };
  
  GLenum TextureFormatToOpenGL(TextureFormat _format);
  TextureFormat OpenGLToTextureFormat(GLenum _format);
  
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
    LINEAR = GL_LINEAR,
    NEAREST = GL_NEAREST
  };
  
  GLenum TextureFilterToOpenGL(TextureFilter _filter);
  TextureFilter OpenGLToTextureFilter(GLenum _filter);

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURETYPES_H_