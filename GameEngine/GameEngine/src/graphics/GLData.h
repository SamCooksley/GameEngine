#ifndef _ENGINE_GRAPHICS_GLDATA_H_
#define _ENGINE_GRAPHICS_GLDATA_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class Depth : GLenum
    {
      NONE,
      NEVER    = GL_NEVER,
      ALWAYS   = GL_ALWAYS,
      LESS     = GL_LESS,
      GREATER  = GL_GREATER,
      EQUAL    = GL_EQUAL,
      NOTEQUAL = GL_NOTEQUAL,
      LEQUAL   = GL_LEQUAL,
      GEQUAL   = GL_GEQUAL,
    };

    Depth OpenGLToDepth(GLenum _depth);
    GLenum DepthToOpenGL(Depth _depth);

    enum class Cull : GLenum
    {
      NONE,
      FRONT = GL_FRONT,
      BACK  = GL_BACK,
      BOTH  = GL_FRONT_AND_BACK
    };

    Cull OpenGLToCull(GLenum _cull);
    GLenum CullToOpenGL(Cull _cull);

    enum class BlendFunc : GLenum
    {
      ZERO                     = GL_ZERO,
      ONE                      = GL_ONE,
      SRC                      = GL_SRC_COLOR,
      ONE_MINUS_SRC            = GL_ONE_MINUS_SRC_COLOR,
      DST                      = GL_DST_COLOR,
      ONE_MINUS_DST            = GL_ONE_MINUS_DST_COLOR,
      SRC_ALPHA                = GL_SRC_ALPHA,
      ONE_MINUS_SRC_ALPHA      = GL_ONE_MINUS_SRC_ALPHA,
      DST_ALPHA                = GL_DST_ALPHA,
      ONE_MINUS_DST_ALPHA      = GL_ONE_MINUS_DST_ALPHA,
      CONSTANT                 = GL_CONSTANT_COLOR,
      ONE_MINUS_CONSTANT       = GL_ONE_MINUS_CONSTANT_COLOR,
      CONSTANT_ALPHA           = GL_CONSTANT_ALPHA,
      ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
      SRC_ALPHA_SATURATE       = GL_SRC_ALPHA_SATURATE,
    };

    BlendFunc OpenGLToBlendFunc(GLenum _blend);
    GLenum BlendFuncToOpenGL(BlendFunc _blend);

    enum class BlendOp : GLenum
    {
      ADD              = GL_FUNC_ADD,
      SUBTRACT         = GL_FUNC_SUBTRACT,
      REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
      MIN              = GL_MIN,
      MAX              = GL_MAX
    };

    BlendOp OpenGLToBlendOp(GLenum _op);
    GLenum BlendOpToOpenGL(BlendOp _op);

    struct Blend
    {
      bool enable;
      BlendFunc src;
      BlendFunc dst;
      BlendOp op;
    };

    class GLData
    {
    public:
      GLData();
      ~GLData();

      void SetDepth(Depth _depth);
      void SetCull(Cull _cull);

      void EnableBlend(BlendFunc _src, BlendFunc _dst, BlendOp _op = BlendOp::ADD);
      void DisableBlend();

      int GetMaxUniformBuffers() const;

      int GetMaxColourAttachments() const;

    private:
      Depth m_depth;
      Cull m_cull;
      Blend m_blend;

      int m_maxUniformBuffers;
      int m_maxColourAttachments;
    };
  }
}

#endif //_ENGINE_GRAPHICS_GLDATA_H_