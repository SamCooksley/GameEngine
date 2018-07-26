#include "stdafx.h"

#include "GLData.h"

namespace engine {
namespace graphics {

  const std::array<String, Depth::COUNT> Depth::s_names = {
    "none",
    "never",  "always",
    "less",   "greater",
    "equal",  "notequal",
    "lequal", "gequal"
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
  
  const String & Depth::ToString(Func _depth)
  {
    if (_depth < 0 || _depth >= COUNT)
    {
      throw std::invalid_argument("Depth.ToString");
    }
  
    return s_names[_depth];
  }
  
  Depth::Func Depth::FromString(const String & _s)
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
  
  const std::array<String, Cull::COUNT> Cull::s_names = {
    "none",
    "front", "back"
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
  
  const String & Cull::ToString(Face _cull)
  {
    if (_cull < 0 || _cull >= COUNT)
    {
      throw std::invalid_argument("Cull.ToString");
    }
  
    return s_names[_cull];
  }
  
  Cull::Face Cull::FromString(const String & _s)
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
  
  const std::array<String, BlendFactor::COUNT> BlendFactor::s_names = {
    "none",
    "zero", "one",
    "srccolour", "oneminussrccolour",
    "srcalpha", "oneminussrcalpha",
    "dstcolour", "oneminusdstcolour",
    "dstalpha", "oneminusdstsalpha"
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
  
  const String & BlendFactor::ToString(Factor _blend)
  {
    if (_blend < 0 || _blend >= COUNT)
    {
      throw std::invalid_argument("BlendFactor.ToString");
    }
  
    return s_names[_blend];
  }
  
  BlendFactor::Factor BlendFactor::FromString(const String & _s)
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
  
  const std::array<String, BlendOp::COUNT + 1> BlendOp::s_names = {
    "none",
    "add",
    "sub", "revsub",
    "min", "max",
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
  
  const String & BlendOp::ToString(Op _blend)
  {
    if (_blend < 0 || _blend >= COUNT)
    {
      throw std::invalid_argument("BlendOp.ToString");
    }
  
    return s_names[_blend];
  }
  
  BlendOp::Op BlendOp::FromString(const String & _s)
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
    return {
      false,
      BlendFactor::ONE,
      BlendFactor::ZERO,
      BlendOp::ADD
    };
  }
  
  const std::array<String, PolygonMode::COUNT> PolygonMode::s_names = {
    "point", "line", "fill"
  };
  
  GLenum PolygonMode::ToOpenGL(Mode _mode)
  {
    switch (_mode)
    {
      case POINT: { return GL_POINT; }
      case LINE:  { return GL_LINE;  }
      case FILL:  { return GL_FILL;  }
      default:
      {
        throw std::invalid_argument("PolygonMode.ToOpenGL");
      }
    }
  }
  
  PolygonMode::Mode PolygonMode::FromOpenGL(GLenum _mode)
  {
    switch (_mode)
    {
      case GL_POINT: { return POINT; }
      case GL_LINE:  { return LINE;  }
      case GL_FILL:  { return FILL;  }
      default:
      {
        throw std::invalid_argument("PolygonMode.FromOpenGL");
      }
    }
  }
  
  const String & PolygonMode::ToString(Mode _blend)
  {
    if (_blend < 0 || _blend >= COUNT)
    {
      throw std::invalid_argument("PolygonMode.ToString");
    }
  
    return s_names[_blend];
  }
  
  PolygonMode::Mode PolygonMode::FromString(const String & _s)
  {
    for (size_t i = 0u; i < s_names.size(); ++i)
    {
      if (string::AreIEqual(s_names[i], _s))
      {
        return static_cast<Mode>(i);
      }
    }
  
    throw std::invalid_argument("PolygonMode.FromString");
  }
  
  GLData::GLData() :
    m_version({ 0, 0 }),
    m_depthWrite(true), m_depth(Depth::NONE),
    m_cull(Cull::NONE), m_polygonMode(PolygonMode::FILL),
    m_blend({ false, BlendFactor::ONE, BlendFactor::ZERO, BlendOp::ADD }),
    m_maxUniformBuffers(36),
    m_maxColourAttachments(4),
    m_maxAnisotropy(0.f)
  { 
    GLint geti2[2];
    GLint & geti = geti2[0];
    GLfloat getf;

    glGetIntegerv(GL_MAJOR_VERSION, &geti);
    m_version.major = geti;

    glGetIntegerv(GL_MINOR_VERSION, &geti);
    m_version.minor = geti;

    glGetIntegerv(GL_CONTEXT_FLAGS, &geti);
    m_contextFlags = geti;
    
    glGetIntegerv(GL_DEPTH_WRITEMASK, &geti);
    m_depthWrite = geti != 0;
  
    if (glIsEnabled(GL_DEPTH_TEST) == GL_FALSE)
    {
      m_depth = Depth::NONE;
    }
    else
    {
      glGetIntegerv(GL_DEPTH_FUNC, &geti);
      m_depth = Depth::FromOpenGL(geti);
    }
  
    if (glIsEnabled(GL_CULL_FACE) == GL_FALSE)
    {
      m_cull = Cull::NONE;
    }
    else
    {
      glGetIntegerv(GL_CULL_FACE_MODE, &geti);
      m_cull = Cull::FromOpenGL(geti);
    }
  
    glGetIntegerv(GL_POLYGON_MODE, geti2);
    m_polygonMode = PolygonMode::FromOpenGL(geti2[0]);
  
    m_blend.enable = glIsEnabled(GL_BLEND) == GL_TRUE;
  
    glGetIntegerv(GL_BLEND_SRC, &geti);
    m_blend.src = BlendFactor::FromOpenGL(geti);
  
    glGetIntegerv(GL_BLEND_DST, &geti);
    m_blend.dst = BlendFactor::FromOpenGL(geti);
  
    glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &geti);
    m_blend.op = BlendOp::FromOpenGL(geti);
    
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &geti);
    m_maxUniformBuffers = glm::max(geti, 0);
  
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &geti);
    m_maxColourAttachments = glm::max(geti, 0);

    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &getf);
    m_maxAnisotropy = getf;
  }
  
  GLData::~GLData()
  { }

  const GLVersion & GLData::getVersion() const
  {
    return m_version;
  }

  int GLData::getContextFlags() const
  {
    return m_contextFlags;
  }
  
  void GLData::SetDepthWrite(bool _write)
  {
    if (_write != m_depthWrite)
    {
      glDepthMask(_write);
      m_depthWrite = _write;
    }
  }
  
  void GLData::SetDepth(Depth::Func _depth)
  {
    if (_depth != m_depth)
    {
      if (_depth == Depth::NONE)
      {
        glDisable(GL_DEPTH_TEST);
      }
      else
      {
        if (m_depth == Depth::NONE)
        {
          glEnable(GL_DEPTH_TEST);
        }
  
        glDepthFunc(Depth::ToOpenGL(_depth));
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
        glDisable(GL_CULL_FACE);
      }
      else
      {
        if (m_cull == Cull::NONE)
        {
          glEnable(GL_CULL_FACE);
        }
        glCullFace(Cull::ToOpenGL(_cull));
      }
      m_cull = _cull;
    }
  }
  
  void GLData::SetPolygonMode(PolygonMode::Mode _mode)
  {
    if (_mode != m_polygonMode)
    {
      glPolygonMode(GL_FRONT_AND_BACK, PolygonMode::ToOpenGL(_mode));
      m_polygonMode = _mode;
    }
  }
  
  void GLData::EnableBlend(BlendFactor::Factor _src, BlendFactor::Factor _dst, BlendOp::Op _op)
  {
    if (!m_blend.enable)
    {
      glEnable(GL_BLEND);
      m_blend.enable = true;
    }
  
    if (_src != m_blend.src || _dst != m_blend.dst)
    {
      glBlendFunc(
        BlendFactor::ToOpenGL(_src),
        BlendFactor::ToOpenGL(_dst)
      );
      m_blend.src = _src;
      m_blend.dst = _dst;
    }
  
    if (_op != m_blend.op)
    {
      glBlendEquation(BlendOp::ToOpenGL(_op));
      m_blend.op = _op;
    }
  }
  
  void GLData::DisableBlend()
  {
    if (m_blend.enable)
    {
      glDisable(GL_BLEND);
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
  
  int GLData::getMaxUniformBuffers() const
  {
    return m_maxUniformBuffers;
  }
  
  int GLData::getMaxColourAttachments() const
  {
    return m_maxColourAttachments;
  }

  float GLData::getMaxAnisotropy() const
  {
    return m_maxAnisotropy;
  }

} } // engine::graphics