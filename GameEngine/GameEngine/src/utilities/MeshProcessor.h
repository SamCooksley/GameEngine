#ifndef _ENGINE_UTILITIES_MESHPROCESSOR_H_
#define _ENGINE_UTILITIES_MESHPROCESSOR_H_

#include "core/Types.h"

#include "graphics\Indices.h"

namespace engine
{
  namespace utilities
  {
    class MeshProcessor
    {
    public:
      enum class Error { OK, MATCHING_UVS };

      MeshProcessor();
      ~MeshProcessor();

      Error CalculateNormals(
        const graphics::Indices & _indices,
        const std::vector<glm::vec3> & _positions,
        std::vector<glm::vec3> * _outNormals
      ) const;

      Error CalculateTangents(
        const graphics::Indices & _indices,
        const std::vector<glm::vec3> & _positions,
        const std::vector<glm::vec2> & _uvs,
        const std::vector<glm::vec3> & _normals,
        std::vector<glm::vec3> * _outTangents,
        std::vector<glm::vec3> * _outBitangents
      ) const;
    };
  }
}

#endif //_ENGINE_UTILITIES_MESHPROCESSOR_H_
