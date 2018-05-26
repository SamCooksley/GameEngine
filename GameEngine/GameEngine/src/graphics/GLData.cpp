#include "stdafx.h"

#include "GLData.h"

namespace engine
{
  namespace graphics
  {
    CullFace OpenGLToCullFace(GLenum _cull)
    {
      switch (_cull)
      {
        case GL_FRONT:          { return CullFace::FRONT; }
        case GL_BACK:           { return CullFace::BACK;  }
        case GL_FRONT_AND_BACK: { return CullFace::BOTH;  }
        default: 
        {
          throw std::invalid_argument("Invalid cull face");
        }
      }
    }

    GLenum CullFaceToOpenGL(CullFace _cull)
    {
      return static_cast<GLenum>(_cull);
    }

    DepthFunc OpenGLToDepthFunc(GLenum _depth)
    {
      switch (_depth)
      {
        case GL_NEVER:    { return DepthFunc::NEVER;    }
        case GL_ALWAYS:   { return DepthFunc::ALWAYS;   }
        case GL_LESS:     { return DepthFunc::LESS;     }
        case GL_GREATER:  { return DepthFunc::GREATER;  }
        case GL_EQUAL:    { return DepthFunc::EQUAL;    }
        case GL_NOTEQUAL: { return DepthFunc::NOTEQUAL; }
        case GL_LEQUAL:   { return DepthFunc::LEQUAL;   }
        case GL_GEQUAL:   { return DepthFunc::GEQUAL;   }
        default:
        {
          throw std::invalid_argument("Invalid depth func");
        }
      }
    }

    GLenum DepthFuncToOpenGL(DepthFunc _depth)
    {
      return static_cast<GLenum>(_depth);
    }

    BlendFunc OpenGLToBlendFunc(GLenum _blend)
    {
      switch (_blend)
      {
        case GL_ZERO:                     { return BlendFunc::ZERO; }
        case GL_ONE:                      { return BlendFunc::ONE; }
        case GL_SRC_COLOR:                { return BlendFunc::SRC; }
        case GL_ONE_MINUS_SRC_COLOR:      { return BlendFunc::ONE_MINUS_SRC; }
        case GL_DST_COLOR:                { return BlendFunc::DST; }
        case GL_ONE_MINUS_DST_COLOR:      { return BlendFunc::ONE_MINUS_DST; }
        case GL_SRC_ALPHA:                { return BlendFunc::SRC_ALPHA; }
        case GL_ONE_MINUS_SRC_ALPHA:      { return BlendFunc::ONE_MINUS_SRC_ALPHA; }
        case GL_DST_ALPHA:                { return BlendFunc::DST_ALPHA; }
        case GL_ONE_MINUS_DST_ALPHA:      { return BlendFunc::ONE_MINUS_DST_ALPHA; }
        case GL_CONSTANT_COLOR:           { return BlendFunc::CONSTANT; }
        case GL_ONE_MINUS_CONSTANT_COLOR: { return BlendFunc::ONE_MINUS_CONSTANT; }
        case GL_CONSTANT_ALPHA:           { return BlendFunc::CONSTANT_ALPHA; }
        case GL_ONE_MINUS_CONSTANT_ALPHA: { return BlendFunc::ONE_MINUS_CONSTANT_ALPHA; }
        case GL_SRC_ALPHA_SATURATE:       { return BlendFunc::SRC_ALPHA_SATURATE; }
        default:
        {
          throw std::invalid_argument("Invalid blend func");
        }
      }
    }

    GLenum BlendFuncToOpenGL(BlendFunc _blend)
    {
      return static_cast<GLenum>(_blend);
    }

    GLData::GLData() : 
      m_depth(false), m_cull(false), m_blend(false),
      m_depthFunc(DepthFunc::LESS), 
      m_cullFace(CullFace::BACK),
      m_blendSrc(BlendFunc::ONE),
      m_blendDst(BlendFunc::ZERO),
      m_maxUniformBuffers(36), 
      m_maxColourAttachments(4)
    { 
      GLCALL(m_depth = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE);
      GLCALL(m_cull = glIsEnabled(GL_CULL_FACE) == GL_TRUE);
      GLCALL(m_blend = glIsEnabled(GL_BLEND) == GL_TRUE);

      GLint get;
      GLCALL(glGetIntegerv(GL_DEPTH_FUNC, &get));
      m_depthFunc = OpenGLToDepthFunc(get);

      GLCALL(glGetIntegerv(GL_CULL_FACE_MODE, &get));
      m_cullFace = OpenGLToCullFace(get);

      GLCALL(glGetIntegerv(GL_BLEND_SRC, &get));
      m_blendSrc = OpenGLToBlendFunc(get);

      GLCALL(glGetIntegerv(GL_BLEND_DST, &get));
      m_blendDst = OpenGLToBlendFunc(get);

      GLCALL(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBuffers));

      GLCALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxColourAttachments));
    }

    GLData::~GLData()
    { }

    void GLData::SetDepth(bool _enable)
    {
      if (_enable != m_depth)
      {
        m_depth = _enable;
        if (m_depth) { GLCALL(glEnable(GL_DEPTH_TEST)); }
        else         { GLCALL(glDisable(GL_DEPTH_TEST)); }
      }
    }

    void GLData::SetDepthFunc(DepthFunc _depth)
    {
      if (_depth != m_depthFunc)
      {
        m_depthFunc = _depth;
        GLCALL(glDepthFunc(DepthFuncToOpenGL(m_depthFunc)));
      }
    }

    void GLData::SetCull(bool _enable)
    {
      if (_enable != m_cull)
      {
        m_cull = _enable;
        if (m_depth) { GLCALL(glEnable(GL_CULL_FACE)); }
        else         { GLCALL(glDisable(GL_CULL_FACE)); }
      }
    }

    void GLData::SetCullFace(CullFace _face)
    {
      if (_face != m_cullFace)
      {
        m_cullFace = _face;
        GLCALL(glCullFace(CullFaceToOpenGL(m_cullFace)));
      }
    }

    void GLData::SetBlend(bool _enable)
    {
      if (_enable != m_blend)
      {
        m_blend = _enable;
        if (m_blend) { GLCALL(glEnable(GL_BLEND)); }
        else         { GLCALL(glDisable(GL_BLEND)); }
      }
    }

    void GLData::SetBlendFunc(BlendFunc _src, BlendFunc _dst)
    {
      if (_src != m_blendSrc || _dst != m_blendDst)
      {
        m_blendSrc = _src;
        m_blendDst = _dst;
        GLCALL(glBlendFunc(
          BlendFuncToOpenGL(m_blendSrc),
          BlendFuncToOpenGL(m_blendDst)
        ));
      }
    }

    int GLData::GetMaxUniformBuffers() const
    {
      return m_maxUniformBuffers;
    }

    int GLData::GetMaxColourAttachments() const
    {
      return m_maxColourAttachments;
    }
  }
}