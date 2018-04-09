#include "stdafx.h"

#include "Object.h"

namespace engine
{
  Object::Object()
  { }

  Object::~Object()
  { }

  const std::string & Object::getName() const
  {
    return m_name;
  }

  void Object::setName(const std::string & _name)
  {
    m_name = _name;
  }

  bool operator==(const Object & _lhs, const Object & _rhs)
  {
    return &_lhs == &_rhs;
  }

  bool operator!=(const Object & _lhs, const Object & _rhs)
  {
    return !(_lhs == _rhs);
  }
}