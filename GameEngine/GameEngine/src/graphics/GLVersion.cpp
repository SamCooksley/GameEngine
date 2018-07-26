#include "stdafx.h"

#include "GLVersion.h"

namespace engine {
namespace graphics {

  bool operator==(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    return _lhs.major == _rhs.major && _lhs.minor == _rhs.minor;
  }

  bool operator!=(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    return !(_lhs == _rhs);
  }

  bool operator<(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    if (_lhs.major == _rhs.major)
    {
      return _lhs.minor < _rhs.minor;
    }

    return _lhs.major < _rhs.major;
  }

  bool operator>(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    if (_lhs.major == _rhs.major)
    {
      return _lhs.minor > _rhs.minor;
    }

    return _lhs.major > _rhs.major;
  }

  bool operator<=(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    if (_lhs.major == _rhs.major)
    {
      return _lhs.minor <= _rhs.minor;
    }

    return _lhs.major <= _rhs.major;
  }

  bool operator>=(const GLVersion & _lhs, const GLVersion & _rhs)
  {
    if (_lhs.major == _rhs.major)
    {
      return _lhs.minor >= _rhs.minor;
    }

    return _lhs.major >= _rhs.major;
  }

} } // engine::graphics