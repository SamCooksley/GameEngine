#include "stdafx.h"

#include "GLData.h"

namespace engine
{
  namespace graphics
  {
    const std::array<std::string, Depth::COUNT + 1> Depth::s_names = {
      "never",  "always",
      "less",   "greater",
      "equal",  "notequal",
      "lequal", "gequal",  
      "none"
    };

    GLenum Depth::ToOpenGL(Func _depth)
    {
      switch (_depth)
      {
        case NEVER:    { return GL_NEVER;    }
        case ALWAYS:   { return GL_ALWAYS;   }
        case LESS:     { return GL_LESS;     }
        case GREATER:  { return GL_GREATER;  }
        case EQUAL:    { return GL_EQUAL;    }
        case NOTEQUAL: { return GL_NOTEQUAL; }
        case LEQUAL:   { return GL_LEQUAL;   }
        case GEQUAL:   { return GL_GEQUAL;   }
        default:
        {
          throw std::invalid_argument("Depth.ToOpenGL");
        }
      }
    }

    Depth::Func Depth::FromOpenGL(GLenum _depth)
    {
      switch (_depth)
      {
        case GL_NEVER:    { return NEVER;    }
        case GL_ALWAYS:   { return ALWAYS;   }
        case GL_LESS:     { return LESS;     }
        case GL_GREATER:  { return GREATER;  }
        case GL_EQUAL:    { return EQUAL;    }
        case GL_NOTEQUAL: { return NOTEQUAL; }
        case GL_LEQUAL:   { return LEQUAL;   }
        case GL_GEQUAL:   { return GEQUAL;   }
        default:          { return NONE;     }
      }
    }

    const std::string & Depth::ToString(Func _depth)
    {
      if (_depth == NONE)
      {
        return s_names.back();
      }

      if (_depth < 0 || _depth >= COUNT)
      {
        throw std::invalid_argument("Depth.ToString");
      }

      return s_names[_depth];
    }

    Depth::Func Depth::FromString(const std::string & _s)
    {
      for (size_t i = 0u; i < s_names.size(); ++i)
      {
        if (string::AreIEqual(s_names[i], _s))
        {
          return static_cast<Func>(i);
        }
      }

      throw std::invalid_argument("Depth.FromString");
    }

    const std::array<std::string, Cull::COUNT + 1> Cull::s_names = {
      "front", "back",
      "none"
    };

    GLenum Cull::ToOpenGL(Face _cull)
    {
      switch (_cull)
      {
        case FRONT: { return GL_FRONT; }
        case BACK:  { return GL_BACK;  }
        default:
        {
          throw std::invalid_argument("Cull.ToOpenGL");
        }
      }
    }

    Cull::Face Cull::FromOpenGL(GLenum _cull)
    {
      switch (_cull)
      {
        case GL_FRONT: { return FRONT; }
        case GL_BACK:  { return BACK;  }
        default:       { return NONE;  }
      }
    }

    const std::string & Cull::ToString(Face _cull)
    {
      if (_cull == NONE)
      {
        return s_names.back();
      }

      if (_cull < 0 || _cull >= COUNT)
      {
        throw std::invalid_argument("Cull.ToString");
      }

      return s_names[_cull];
    }

    Cull::Face Cull::FromString(const std::string & _s)
    {
      for (size_t i = 0u; i < s_names.size(); ++i)
      {
        if (string::AreIEqual(s_names[i], _s))
        {
          return static_cast<Face>(i);
        }
      }

      throw std::invalid_argument("Cull.FromString");
    }

    const std::array<std::string, BlendFactor::COUNT + 1> BlendFactor::s_names = {
      "zero", "one",
      "srccolour", "oneminussrccolour",
      "srcalpha", "oneminussrcalpha",
      "dstcolour", "oneminusdstcolour",
      "dstalpha", "oneminusdstsalpha",
      "none"
    };

    GLenum BlendFactor::ToOpenGL(Factor _blend)
    {
      switch (_blend)
      {
        case ZERO:                 { return GL_ZERO;                }
        case ONE:                  { return GL_ONE;                 }
        case SRC_COLOUR:           { return GL_SRC_COLOR;           }
        case ONE_MINUS_SRC_COLOUR: { return GL_ONE_MINUS_SRC_COLOR; }
        case SRC_ALPHA:            { return GL_SRC_ALPHA;           }
        case ONE_MINUS_SRC_ALPHA:  { return GL_ONE_MINUS_SRC_ALPHA; }
        case DST_COLOUR:           { return GL_DST_COLOR;           }
        case ONE_MINUS_DST_COLOUR: { return GL_ONE_MINUS_DST_COLOR; }
        case DST_ALPHA:            { return GL_DST_ALPHA;           }
        case ONE_MINUS_DST_ALPHA:  { return GL_ONE_MINUS_DST_ALPHA; }
        default:
        {
          throw std::invalid_argument("BlendFactor.ToOpenGL");
        }
      }
    }

    BlendFactor::Factor BlendFactor::FromOpenGL(GLenum _blend)
    {
      switch (_blend)
      {
        case GL_ZERO:                { return ZERO;                 }
        case GL_ONE:                 { return ONE;                  }
        case GL_SRC_COLOR:           { return SRC_COLOUR;           }
        case GL_ONE_MINUS_SRC_COLOR: { return ONE_MINUS_SRC_COLOUR; }
        case GL_SRC_ALPHA:           { return SRC_ALPHA;            }
        case GL_ONE_MINUS_SRC_ALPHA: { return ONE_MINUS_SRC_ALPHA;  }
        case GL_DST_COLOR:           { return DST_COLOUR;           }
        case GL_ONE_MINUS_DST_COLOR: { return ONE_MINUS_DST_COLOUR; }
        case GL_DST_ALPHA:           { return DST_ALPHA;            }
        case GL_ONE_MINUS_DST_ALPHA: { return ONE_MINUS_DST_ALPHA;  }
        default:                     { return ZERO;                 }
      }
    }

    const std::string & BlendFactor::ToString(Factor _blend)
    {
      if (_blend == NONE)
      {
        return s_names.back();
      }

      if (_blend < 0 || _blend >= COUNT)
      {
        throw std::invalid_argument("BlendFactor.ToString");
      }

      return s_names[_blend];
    }

    BlendFactor::Factor BlendFactor::FromString(const std::string & _s)
    {
      for (size_t i = 0u; i < s_names.size(); ++i)
      {
        if (string::AreIEqual(s_names[i], _s))
        {
          return static_cast<Factor>(i);
        }
      }

      throw std::invalid_argument("BlendFactor.FromString");
    }

    const std::array<std::string, BlendOp::COUNT + 1> BlendOp::s_names = {
      "add",
      "sub", "revsub",
      "min", "max",
      "none"
    };

    GLenum BlendOp::ToOpenGL(Op _op)
    {
      switch (_op)
      {
        case ADD:     { return GL_FUNC_ADD;              }
        case SUB:     { return GL_FUNC_SUBTRACT;         }
        case REV_SUB: { return GL_FUNC_REVERSE_SUBTRACT; }
        case MIN:     { return GL_MIN;                   }
        case MAX:     { return GL_MAX;                   }
        default:
        {
          throw std::invalid_argument("BlendOp.ToOpenGL");
        }
      }
    }

    BlendOp::Op BlendOp::FromOpenGL(GLenum _op)
    {
      switch (_op)
      {
        case GL_FUNC_ADD:              { return ADD;     }
        case GL_FUNC_SUBTRACT:         { return SUB;     }
        case GL_FUNC_REVERSE_SUBTRACT: { return REV_SUB; }
        case GL_MIN:                   { return MIN;     }
        case GL_MAX:                   { return MAX;     }
        default:                       { return NONE;    }
      }
    }

    const std::string & BlendOp::ToString(Op _blend)
    {
      if (_blend == NONE)
      {
        return s_names.back();
      }

      if (_blend < 0 || _blend >= COUNT)
      {
        throw std::invalid_argument("BlendOp.ToString");
      }

      return s_names[_blend];
    }

    BlendOp::Op BlendOp::FromString(const std::string & _s)
    {
      for (size_t i = 0u; i < s_names.size(); ++i)
      {
        if (string::AreIEqual(s_names[i], _s))
        {
          return static_cast<Op>(i);
        }
      }

      throw std::invalid_argument("BlendOp.FromString");
    }

    Blend Blend::Disable()
    {
      const Blend blend = {
        false,
        BlendFactor::ONE,
        BlendFactor::ZERO,
        BlendOp::ADD
      };
      return blend;
    }

    GLData::GLData() : 
      m_depth(Depth::NONE), m_cull(Cull::NONE), 
      m_blend({ false, BlendFactor::ONE, BlendFactor::ZERO, BlendOp::ADD }),
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
        m_depth = Depth::FromOpenGL(get);
      }

      GLCALL(enable = glIsEnabled(GL_CULL_FACE) == GL_TRUE);
      if (!enable)
      {
        m_cull = Cull::NONE;
      }
      else
      {
        GLCALL(glGetIntegerv(GL_CULL_FACE_MODE, &get));
        m_cull = Cull::FromOpenGL(get);
      }

      GLCALL(m_blend.enable = glIsEnabled(GL_BLEND) == GL_TRUE);

      GLCALL(glGetIntegerv(GL_BLEND_SRC, &get));
      m_blend.src = BlendFactor::FromOpenGL(get);

      GLCALL(glGetIntegerv(GL_BLEND_DST, &get));
      m_blend.dst = BlendFactor::FromOpenGL(get);

      GLCALL(glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &get));
      m_blend.op = BlendOp::FromOpenGL(get);
      
      GLCALL(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &get));
      m_maxUniformBuffers = glm::max(get, 0);

      GLCALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &get));
      m_maxColourAttachments = glm::max(get, 0);
    }

    GLData::~GLData()
    { }

    void GLData::SetDepth(Depth::Func _depth)
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

          GLCALL(glDepthFunc(Depth::ToOpenGL(_depth)));
        }
        m_depth = _depth;
      }
    }

    void GLData::SetCull(Cull::Face _cull)
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

          GLCALL(glCullFace(Cull::ToOpenGL(_cull)));
        }
        m_cull = _cull;
      }
    }

    void GLData::EnableBlend(BlendFactor::Factor _src, BlendFactor::Factor _dst, BlendOp::Op _op)
    {
      if (!m_blend.enable)
      {
        GLCALL(glEnable(GL_BLEND));
        m_blend.enable = true;
      }

      if (_src != m_blend.src || _dst != m_blend.dst)
      {
        GLCALL(glBlendFunc(
          BlendFactor::ToOpenGL(_src),
          BlendFactor::ToOpenGL(_dst)
        ));
        m_blend.src = _src;
        m_blend.dst = _dst;
      }

      if (_op != m_blend.op)
      {
        GLCALL(glBlendEquation(BlendOp::ToOpenGL(_op)));
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

    void GLData::SetBlend(const Blend & _blend)
    {
      if (_blend.enable)
      {
        EnableBlend(_blend.src, _blend.dst, _blend.op);
      }
      else
      {
        DisableBlend();
      }
    }

    uint GLData::GetMaxUniformBuffers() const
    {
      return m_maxUniformBuffers;
    }

    uint GLData::GetMaxColourAttachments() const
    {
      return m_maxColourAttachments;
    }
  }
}