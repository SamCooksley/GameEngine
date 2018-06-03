#ifndef _ENGINE_GRAPHICS_TYPEDATA_H_
#define _ENGINE_GRAPHICS_TYPEDATA_H_

#include "Types.h"

namespace engine {
namespace graphics {

  template <typename T>
  struct TypeData
  { };
  
  template <>
  struct TypeData<int>
  {
    static constexpr Type type = Type::INT;
    static constexpr ComponentType component = ComponentType::INT;
    static constexpr uint count = 1;
  };
  
  template <>
  struct TypeData<float>
  {
    static constexpr Type type = Type::FLOAT;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 1;
  };
  
  template <>
  struct TypeData<glm::vec2>
  {
    static constexpr Type type = Type::VEC2;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 2;
  };
  
  template <>
  struct TypeData<glm::vec3>
  {
    static constexpr Type type = Type::VEC3;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 3;
  };
  
  template <>
  struct TypeData<glm::vec4>
  {
    static constexpr Type type = Type::VEC4;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 4;
  };
  
  template <>
  struct TypeData<glm::mat3>
  {
    static constexpr Type type = Type::MAT3;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 9;
  };
  
  template <>
  struct TypeData<glm::mat4>
  {
    static constexpr Type type = Type::MAT4;
    static constexpr ComponentType component = ComponentType::FLOAT;
    static constexpr uint count = 16;
  };
  
  ComponentType GetComponentType(Type _type);
  uint GetComponentCount(Type _type);
  uint GetTypeSize(Type _type);

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TYPEDATA_H_