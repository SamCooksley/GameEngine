#include "stdafx.h"

#include "TypeData.h"

namespace engine {
namespace graphics {

  ComponentType GetComponentType(Type _type)
  {
    switch (_type)
    {
      case Type::INT:   { return TypeData<int>::component;       }
      case Type::FLOAT: { return TypeData<float>::component;     }
      case Type::VEC2:  { return TypeData<glm::vec2>::component; }
      case Type::VEC3:  { return TypeData<glm::vec3>::component; }
      case Type::VEC4:  { return TypeData<glm::vec4>::component; }
      case Type::MAT3:  { return TypeData<glm::mat3>::component; }
      case Type::MAT4:  { return TypeData<glm::mat4>::component; }
      default: 
      {
        throw std::invalid_argument("Invalid type");
      }
    }
  }
  
  uint GetComponentCount(Type _type)
  {
    switch (_type)
    {
      case Type::INT:   { return TypeData<int>::count;       }
      case Type::FLOAT: { return TypeData<float>::count;     }
      case Type::VEC2:  { return TypeData<glm::vec2>::count; }
      case Type::VEC3:  { return TypeData<glm::vec3>::count; }
      case Type::VEC4:  { return TypeData<glm::vec4>::count; }
      case Type::MAT3:  { return TypeData<glm::mat3>::count; }
      case Type::MAT4:  { return TypeData<glm::mat4>::count; }
      default: 
      { 
        throw std::invalid_argument("Invalid type");
      }
    }
  }
  
  uint GetTypeSize(Type _type)
  {
    switch (_type)
    {
      case Type::INT:   { return sizeof(int);       }
      case Type::FLOAT: { return sizeof(float);     }
      case Type::VEC2:  { return sizeof(glm::vec2); }
      case Type::VEC3:  { return sizeof(glm::vec3); }
      case Type::VEC4:  { return sizeof(glm::vec4); }
      case Type::MAT3:  { return sizeof(glm::mat3); }
      case Type::MAT4:  { return sizeof(glm::mat4); }
      default: 
      {
        throw std::invalid_argument("Invalid type");
      }
    }
  }

} } // engine::graphics