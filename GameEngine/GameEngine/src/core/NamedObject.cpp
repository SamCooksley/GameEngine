#include "stdafx.h"

#include "NamedObject.h"

namespace engine {

  NamedObject::NamedObject(const String & _name) :
    m_name(_name)
  { }

  void NamedObject::setName(const String & _name)
  {
    m_name = _name;
  }

  const String & NamedObject::getName() const
  {
    return m_name;
  }

} // engine