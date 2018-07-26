#ifndef _ENGINE_GRAPHICS_GLVERSION_H_
#define _ENGINE_GRAPHICS_GLVERSION_H_

namespace engine {
namespace graphics {

  struct GLVersion
  {
    int major;
    int minor;
  };

  bool operator==(const GLVersion & _lhs, const GLVersion & _rhs);
  bool operator!=(const GLVersion & _lhs, const GLVersion & _rhs);

  bool operator<(const GLVersion & _lhs, const GLVersion & _rhs);
  bool operator>(const GLVersion & _lhs, const GLVersion & _rhs);

  bool operator<=(const GLVersion & _lhs, const GLVersion & _rhs);
  bool operator>=(const GLVersion & _lhs, const GLVersion & _rhs);

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_GLVERSION_H_