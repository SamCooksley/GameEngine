#ifndef _ENGINE_CORE_NAMEDOBJECT_H_
#define _ENGINE_CORE_NAMEDOBJECT_H_

#include "String.h"

namespace engine
{
  class NamedObject
  {
  public:
    NamedObject() = default;
    NamedObject(std::string _name);
    virtual ~NamedObject() = default;

    void setName(std::string _name);
    const std::string & getName() const;

  private:
    std::string m_name;
  };
}

#endif //_ENGINE_CORE_NAMEDOBJECT_H_