#include "stdafx.h"

#include "DebugOutput.h"

#include <sstream>
#include <iomanip>

#include "Graphics.h"

namespace engine {
namespace graphics {

  const char * DebugSource::ToString(DebugSource::Type _source)
  {
    switch (_source)
    {
      case DebugSource::API:             { return "API";             }
      case DebugSource::WINDOWS_SYSTEM:  { return "Windows";         }
      case DebugSource::SHADER_COMPILER: { return "Shader Compiler"; }
      case DebugSource::THRID_PARTY:     { return "Third Party";     }
      case DebugSource::APPLICATION:     { return "Application";     }
      default:                           { return "Other";           }
    }
  }

  const char * DebugType::ToString(DebugType::Type _type)
  {
    switch (_type)
    {
      case DebugType::ERROR:                { return "Error";                }
      case DebugType::DEPRECATED_BEHAVIOUR: { return "Deprecated Behaviour"; }
      case DebugType::UNDEFINED_BEHAVIOUR:  { return "Undefined Behaviour";  }
      case DebugType::PERFORMANCE:          { return "Performance";          }
      case DebugType::PORTABILITY:          { return "Portability";          }
      case DebugType::MARKER:               { return "Marker";               }
      case DebugType::PUSH_GROUP:           { return "Push group";           }
      case DebugType::POP_GROUP:            { return "Pop group";            }
      default:                              { return "Other";                }
    }
  }

  const char * DebugSeverity::ToString(DebugSeverity::Type _severity)
  {
    switch (_severity)
    {
      case DebugSeverity::HIGH:         { return "High";         }
      case DebugSeverity::MEDUIM:       { return "Medium";       }
      case DebugSeverity::LOW:          { return "Low";          }
      case DebugSeverity::NOTIFICATION: { return "Notification"; }
      default:                          { return "Unknown";      }
    }
  }

  void GLAPIENTRY DebugOutput::Log(
    GLenum _source, GLenum _type, GLuint _id, GLenum _severity,
    GLsizei _length, const GLchar * _message,
    const void * _userParam
  )
  {
    if (_userParam != nullptr)
    {
      reinterpret_cast<const DebugOutput *>(_userParam)->Log(
        _source, _type, _id, _severity,
        _length, _message
      );
      return;
    }
    else
    {
      std::stringstream message;
      message << "Source: "     << DebugSource::ToString(static_cast<DebugSource::Type>(_source))
              << "\nType: "     << DebugType::ToString(static_cast<DebugType::Type>(_type))
              << "\nSeverity: " << DebugSeverity::ToString(static_cast<DebugSeverity::Type>(_severity))
              << "\nID: "       << _id
              << "\nMessage:\n" << _message
              << '\n';

      debug::Log(message.str());
    }
  }

  DebugOutput::DebugOutput() :
    m_maxGroupDepth(64),
    m_enabled(false),
    m_depth(0),
    m_tabSize(2),
    m_maxIndent(25)
  {
    {
      GLint geti;
      glGetIntegerv(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &geti);
      m_maxGroupDepth = geti;
    }

    if (Graphics::GL().getContextFlags() & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(DebugOutput::Log, this);
      glDebugMessageControl(DebugSource::ANY, DebugType::ANY, DebugSeverity::ANY, 0, nullptr, true);
      
      std::stringstream message;
      message << "Debug output enabled\n"
              << "OpenGL version: " << glGetString(GL_VERSION) << '\n'
              << "GLSL version: "   << glGetString(GL_SHADING_LANGUAGE_VERSION);

      std::string m = message.str();

      Log(DebugType::MARKER, DebugSeverity::NOTIFICATION, 0, m.c_str(), m.size());

      m_enabled = true;
    }
  }

  DebugOutput::~DebugOutput()
  {
    if (m_enabled)
    {
      //continue to output messages 
      glDebugMessageCallback(DebugOutput::Log, nullptr);
    }
  }

  void DebugOutput::Log(DebugType::Type _type, DebugSeverity::Type _severity, uint _id, const char * _message, size_t _length)
  {
    glDebugMessageInsert(DebugSource::APPLICATION, _type, _id, _severity, _length, _message);
  }

  void DebugOutput::PushGroup(uint _id, const char * _message, size_t _length)
  {
    if (m_depth >= m_maxGroupDepth)
    {
      throw std::overflow_error("max group depth reached");
    }

    glPushDebugGroup(DebugSource::APPLICATION, _id, _length, _message);
    ++m_depth;

    UpdateIndent();
  }

  void DebugOutput::PopGroup()
  {
    glPopDebugGroup();
    --m_depth;

    UpdateIndent();
  }

  void DebugOutput::AddFilter(
    bool _enabled,
    DebugSource::Type _source, DebugType::Type _type, DebugSeverity::Type _severity,
    const std::vector<uint> & _ids
  )
  {
    glDebugMessageControl(_source, _type, _severity, _ids.size(), _ids.data(), _enabled);
  }

  void DebugOutput::Log(
    GLenum _source, GLenum _type, GLuint _id, GLenum _severity,
    GLsizei _length, const GLchar * _message
  ) const
  {
    std::stringstream message;
    message << m_indent << "Source: "   << DebugSource::ToString(static_cast<DebugSource::Type>(_source))     << '\n'
            << m_indent << "Type: "     << DebugType::ToString(static_cast<DebugType::Type>(_type))             << '\n'
            << m_indent << "Severity: " << DebugSeverity::ToString(static_cast<DebugSeverity::Type>(_severity)) << '\n'
            << m_indent << "ID: "       << _id << '\n'
            << m_indent << "Message:\n" 
            << m_indent << _message << '\n';

    debug::Log(message.str());
  }

  void DebugOutput::UpdateIndent()
  {
    m_indent = String(glm::min(m_depth, m_maxIndent) * m_tabSize, ' ');
  }

} } // engine::graphics