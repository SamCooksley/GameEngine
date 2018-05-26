#include "stdafx.h"

#include "GLData.h"

namespace engine
{
  namespace graphics
  {
    Cull OpenGLToCull(GLenum _cull)
    {
      switch (_cull)
      {
        case GL_FRONT:          { return Cull::FRONT; }
        case GL_BACK:           { return Cull::BACK;  }
        case GL_FRONT_AND_BACK: { return Cull::BOTH;  }
        default:                { return Cull::NONE;  }
      }
    }

    GLenum CullToOpenGL(Cull _cull)
    {
      return static_cast<GLenum>(_cull);
    }

    Depth OpenGLToDepth(GLenum _depth)
    {
      switch (_depth)
      {
        case GL_NEVER:    { return Depth::NEVER;    }
        case GL_ALWAYS:   { return Depth::ALWAYS;   }
        case GL_LESS:     { return Depth::LESS;     }
        case GL_GREATER:  { return Depth::GREATER;  }
        case GL_EQUAL:    { return Depth::EQUAL;    }
        case GL_NOTEQUAL: { return Depth::NOTEQUAL; }
        case GL_LEQUAL:   { return Depth::LEQUAL;   }
        case GL_GEQUAL:   { return Depth::GEQUAL;   }
        default:          { return Depth::NONE;     }
      }
    }

    GLenum DepthToOpenGL(Depth _depth)
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
          throw std::invalid_argument("Invalid blend function");
        }
      }
    }

    GLenum BlendFuncToOpenGL(BlendFunc _blend)
    {
      return static_cast<GLenum>(_blend);
    }

    BlendOp OpenGLToBlendOp(GLenum _op)
    {
      switch (_op)
      {
        case GL_FUNC_ADD:              { return BlendOp::ADD; }
        case GL_FUNC_SUBTRACT:         { return BlendOp::SUBTRACT; }
        case GL_FUNC_REVERSE_SUBTRACT: { return BlendOp::REVERSE_SUBTRACT; }
        case GL_MIN:                   { return BlendOp::MIN; }
        case GL_MAX:                   { return BlendOp::MAX; }
        default: 
        {
          throw std::invalid_argument("Invalid blend operation");
        }
      }
    }

    GLenum BlendOpToOpenGL(BlendOp _op)
    {
      return static_cast<GLenum>(_op);
    }

    GLData::GLData() : 
      m_depth(Depth::NONE), m_cull(Cull::NONE), 
      m_blend({ false, BlendFunc::ONE, BlendFunc::ZERO, BlendOp::ADD }),
      m_maxUniformBuffers(36), 
      m_maxColourAttachments(4)
    { 
      bool enable;
      GLint get;

      GLCALL(enable = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE);
      if (!enable)
      {
        m_depth = Depth::NONE;
      }
      else
      {
        GLCALL(glGetIntegerv(GL_DEPTH_FUNC, &get));
        m_depth = OpenGLToDepth(get);
      }

      GLCALL(enable = glIsEnabled(GL_CULL_FACE) == GL_TRUE);
      if (!enable)
      {
        m_cull = Cull::NONE;
      }
      else
      {
        GLCALL(glGetIntegerv(GL_CULL_FACE_MODE, &get));
        m_cull = OpenGLToCull(get);
      }

      GLCALL(m_blend.enable = glIsEnabled(GL_BLEND) == GL_TRUE);

      GLCALL(glGetIntegerv(GL_BLEND_SRC, &get));
      m_blend.src = OpenGLToBlendFunc(get);

      GLCALL(glGetIntegerv(GL_BLEND_DST, &get));
      m_blend.dst = OpenGLToBlendFunc(get);

      GLCALL(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &get));
      m_blend.op = OpenGLToBlendOp(get);

      GLCALL(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_maxUniformBuffers));

      GLCALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &m_maxColourAttachments));
    }

    GLData::~GLData()
    { }

    void GLData::SetDepth(Depth _depth)
    {
      if (_depth != m_depth)
      {
        if (_depth == Depth::NONE)
        {
          GLCALL(glDisable(GL_DEPTH_TEST));
        }
        else
        {
          if (m_depth == Depth::NONE)
          {
            GLCALL(glEnable(GL_DEPTH_TEST));
          }

          GLCALL(glDepthFunc(DepthToOpenGL(_depth)));
        }
        m_depth = _depth;
      }
    }

    void GLData::SetCull(Cull _cull)
    {
      if (_cull != m_cull)
      {
        if (_cull == Cull::NONE)
        {
          GLCALL(glDisable(GL_CULL_FACE));
        }
        else
        {
          if (m_cull == Cull::NONE)
          {
            GLCALL(glEnable(GL_CULL_FACE));
          }

          GLCALL(glCullFace(CullToOpenGL(_cull)));
        }
        m_cull = _cull;
      }
    }

    void GLData::EnableBlend(BlendFunc _src, BlendFunc _dst, BlendOp _op)
    {
      if (!m_blend.enable)
      {
        GLCALL(glEnable(GL_BLEND));
        m_blend.enable = true;
      }

      if (_src != m_blend.src || _dst != m_blend.dst)
      {
        GLCALL(glBlendFunc(
          BlendFuncToOpenGL(_src),
          BlendFuncToOpenGL(_dst)
        ));
        m_blend.src = _src;
        m_blend.dst = _dst;
      }

      if (_op != m_blend.op)
      {
        GLCALL(glBlendEquation(BlendOpToOpenGL(_op)));
        m_blend.op = _op;
      }
    }

    void GLData::DisableBlend()
    {
      if (m_blend.enable)
      {
        GLCALL(glDisable(GL_BLEND));
        m_blend.enable = false;
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