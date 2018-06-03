#ifndef _ENGINE_DEBUG_H_
#define _ENGINE_DEBUG_H_

#include "core\Types.h"

namespace engine {
namespace debug {

  void Log(const String & _message);
  void LogWarning(const String & _message);
  void LogError(const String & _message);

} } // engine::debug

#endif //_ENGINE_DEBUG_H_