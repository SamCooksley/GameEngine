#ifndef _ENGINE_CORE_TYPES_H_
#define _ENGINE_CORE_TYPES_H_

#include <cstdint>

#include "String.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "NonCopyable.h"
#include "NamedObject.h"

#include "Dictionary.h"

#include "not_implemented.h"

namespace engine {

  using int8  = std::int8_t;
  using int16 = std::int16_t;
  using int32 = std::int32_t;
  using int64 = std::int64_t;
  
  using uint8  = std::uint8_t;
  using uint16 = std::uint16_t;
  using uint32 = std::uint32_t;
  using uint64 = std::uint64_t;
  
  using byte = int8;
  using uint = uint32;

} // engine

#endif //_ENGINE_CORE_TYPES_H_