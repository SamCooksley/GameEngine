#ifndef _ENGINE_RESOURCES_H_
#define _ENGINE_RESOURCES_H_

#include "Application.h"

#include "Asset.h"

namespace engine
{
  class Resources
  {
  public:
    Resources() = delete;

    template <class T>
    static std::shared_ptr<T> Load(const std::string & _path);    

    static void Clear();
  };
}

#include "Resources.inl"

#endif //_ENGINE_RESOURCES_H_