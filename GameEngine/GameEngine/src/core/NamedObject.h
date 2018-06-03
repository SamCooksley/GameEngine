#ifndef _ENGINE_CORE_NAMEDOBJECT_H_
#define _ENGINE_CORE_NAMEDOBJECT_H_

#include "String.h"

namespace engine {

  class NamedObject
  {
   public:
    NamedObject() = default;
    NamedObject(const String & _name);
    virtual ~NamedObject() = default;
  
    void setName(const String & _name);
    const String & getName() const;
  
   private:
    String m_name;
  };

} // engine

#endif //_ENGINE_CORE_NAMEDOBJECT_H_