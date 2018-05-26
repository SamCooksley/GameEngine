#ifndef _ENGINE_GRAPHICS_GLDATA_H_
#define _ENGINE_GRAPHICS_GLDATA_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    enum class CullFace : GLenum
    {
      FRONT = GL_FRONT,
      BACK  = GL_BACK,
      BOTH  = GL_FRONT_AND_BACK
    };

    CullFace OpenGLToCullFace(GLenum _cull);
    GLenum CullFaceToOpenGL(CullFace _cull);

    enum class DepthFunc : GLenum
    {
      NEVER    = GL_NEVER,
      ALWAYS   = GL_ALWAYS,
      LESS     = GL_LESS,
      GREATER  = GL_GREATER,
      EQUAL    = GL_EQUAL,
      NOTEQUAL = GL_NOTEQUAL,
      LEQUAL   = GL_LEQUAL,
      GEQUAL   = GL_GEQUAL,
    };

    DepthFunc OpenGLToDepthFunc(GLenum _depth);
    GLenum DepthFuncToOpenGL(DepthFunc _depth);

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

    class GLData
    {
    public:
      GLData();
      ~GLData();

      void SetDepth(bool _enable);
      void SetDepthFunc(DepthFunc _depth);

      void SetCull(bool _enable);
      void SetCullFace(CullFace _face);

      void SetBlend(bool _enable);
      void SetBlendFunc(BlendFunc _src, BlendFunc _dst);

      int GetMaxUniformBuffers() const;

      int GetMaxColourAttachments() const;

    private:
      bool m_depth;
      bool m_cull;
      bool m_blend;

      DepthFunc m_depthFunc;

      CullFace m_cullFace;

      BlendFunc m_blendSrc;
      BlendFunc m_blendDst;

      int m_maxUniformBuffers;
      int m_maxColourAttachments;
    };
  }
}

#endif //_ENGINE_GRAPHICS_GLDATA_H_