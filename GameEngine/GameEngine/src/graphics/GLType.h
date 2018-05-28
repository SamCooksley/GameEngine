#ifndef _ENGINE_GRAPHICS_GLTYPE_H_
#define _ENGINE_GRAPHICS_GLTYPE_H_

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    template <typename T>
    class GLType
    {
    public:
      virtual T ToOpenGL() const { return m_value; }

    protected:
      GLType() :
        m_value(0) { }
      GLType(const T & _value) : 
        m_value(_value) { }
      virtual ~GLType() { }

      T m_value;

      friend inline bool operator==(const GLType<T> & _lhs, const GLType<T> & _rhs)
      {
        return _lhs.m_value == _rhs.m_value;
      }

      friend inline bool operator!=(const GLType<T> & _lhs, const GLType<T> & _rhs)
      {
        return _lhs.m_value != _rhs.m_value;
      }

      friend inline bool operator<(const GLType<T> & _lhs, const GLType<T> & _rhs)
      {
        return _lhs.m_value < _rhs.m_value;
      }
    };
  }
}

#endif //_ENGINE_GRAPHICS_GLTYPE_H_