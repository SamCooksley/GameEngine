#ifndef _ENGINE_GRAPHICS_MESHDATA_H_
#define _ENGINE_GRAPHICS_MESHDATA_H_

#include "Indices.h"

namespace engine
{
  namespace graphics
  {
    struct MeshData
    {
    public:
      Indices indices;
      std::vector<glm::vec3> positions;
      std::vector<glm::vec2> uvs;
      std::vector<glm::vec3> normals;
      std::vector<glm::vec3> tangents;
      std::vector<glm::vec3> bitangents;

      bool HasIndicies() const;
      bool HasUVs() const;
      bool HasNormals() const;
      bool HasTangents() const;
    };
  }
}

#endif //_ENGINE_GRAPHICS_MESHDATA_H_