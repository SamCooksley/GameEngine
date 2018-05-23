#ifndef _ENGINE_DEBUG_H_
#define _ENGINE_DEBUG_H_

#include "core\Types.h"

namespace engine
{
  namespace debug
  {
    void Log(const std::string & _message);
    void LogWarning(const std::string & _message);
    void LogError(const std::string & _message);
  }
}

#endif //_ENGINE_DEBUG_H_