#include "stdafx.h"

#include "NamedObject.h"

namespace engine
{
  NamedObject::NamedObject(std::string _name) :
    m_name(std::move(_name))
  { }

  void NamedObject::setName(std::string _name)
  {
    m_name = std::move(_name);
  }

  const std::string & NamedObject::getName() const
  {
    return m_name;
  }
}