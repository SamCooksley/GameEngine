#include "stdafx.h"

#include "Object.h"

namespace engine { 

  Object::Object()
  { }

  Object::~Object()
  { }

  bool operator==(const Object & _lhs, const Object & _rhs)
  {
    return &_lhs == &_rhs;
  }

  bool operator!=(const Object & _lhs, const Object & _rhs)
  {
    return !(_lhs == _rhs);
  }

} // engine