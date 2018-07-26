#ifndef _ENGINE_GRAPHICS_DEBUGOUTPUT_H_
#define _ENGINE_GRAPHICS_DEBUGOUTPUT_H_

#include "opengl.h"

namespace engine {
namespace graphics {

  namespace DebugSource
  {
    enum Type
    {
      API = GL_DEBUG_SOURCE_API,
      WINDOWS_SYSTEM = GL_DEBUG_SOURCE_WINDOW_SYSTEM,
      SHADER_COMPILER = GL_DEBUG_SOURCE_SHADER_COMPILER,
      THRID_PARTY = GL_DEBUG_SOURCE_THIRD_PARTY,
      APPLICATION = GL_DEBUG_SOURCE_APPLICATION,
      OTHER = GL_DEBUG_SOURCE_OTHER,
      ANY = GL_DONT_CARE
    };

    const char * ToString(Type _source);
  };

  namespace DebugType
  {
    enum Type
    {
      ERROR = GL_DEBUG_TYPE_ERROR,
      DEPRECATED_BEHAVIOUR = GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
      UNDEFINED_BEHAVIOUR = GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
      PERFORMANCE = GL_DEBUG_TYPE_PERFORMANCE,
      PORTABILITY = GL_DEBUG_TYPE_PORTABILITY,
      MARKER = GL_DEBUG_TYPE_MARKER,
      PUSH_GROUP = GL_DEBUG_TYPE_PUSH_GROUP,
      POP_GROUP = GL_DEBUG_TYPE_POP_GROUP,
      OTHER = GL_DEBUG_TYPE_OTHER,
      ANY = GL_DONT_CARE
    };

    const char * ToString(Type _type);
  };

  namespace DebugSeverity
  {
    enum Type
    {
      HIGH = GL_DEBUG_SEVERITY_HIGH,
      MEDUIM = GL_DEBUG_SEVERITY_MEDIUM,
      LOW = GL_DEBUG_SEVERITY_LOW,
      NOTIFICATION = GL_DEBUG_SEVERITY_NOTIFICATION,
      ANY = GL_DONT_CARE
    };

    const char * ToString(Type _severity);
  };

  class DebugOutput
  {
   private:
    static void GLAPIENTRY Log(
      GLenum _source, GLenum _type, GLuint _id, GLenum _severity,
      GLsizei _length, const GLchar * _message,
      const void * _userParam
    );

   public:
    DebugOutput();
    ~DebugOutput();

    void Log(DebugType::Type _type, DebugSeverity::Type _severity, uint _id, const char * _message, size_t _length);
    
    void PushGroup(uint _id, const char * _message, size_t _length);
    void PopGroup();

    void AddFilter(
      bool _enabled,
      DebugSource::Type _source = DebugSource::ANY, 
      DebugType::Type _type = DebugType::ANY,
      DebugSeverity::Type _severity = DebugSeverity::ANY, 
      const std::vector<uint> & _ids = { }
    );

   private:
    void Log(
      GLenum _source, GLenum _type, GLuint _id, GLenum _severity,
      GLsizei _length, const GLchar * _message
    ) const;

    void UpdateIndent();

   private:
    int m_maxGroupDepth;

    bool m_enabled;

    int m_depth;
    int m_tabSize;
    int m_maxIndent;
    String m_indent;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_DEBUGOUTPUT_H_