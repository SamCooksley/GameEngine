#include "stdafx.h"

#include "MeshProcessor.h"

#include "debug\Debug.h"

namespace engine
{
  namespace utilities
  {
    MeshProcessor::MeshProcessor()
    { }

    MeshProcessor::~MeshProcessor()
    { }

    void MeshProcessor::CalculateTangents(
      const std::vector<uint32> & _indices,
      const std::vector<glm::vec3> & _positions,
      const std::vector<glm::vec2> & _uvs,
      const std::vector<glm::vec3> & _normals,
      std::vector<glm::vec3> * _outTangents,
      std::vector<glm::vec3> * _outBitangents
    ) const
    {
      if (_outTangents == nullptr ||
        _outBitangents == nullptr)
      {
        throw std::invalid_argument("MeshProcessor Error: missing output tangents or bitangents.");
      }

      _outTangents->resize(_positions.size());
      _outBitangents->resize(_positions.size());

      for (size_t i = 0; i < _indices.size(); i += 3)
      {
        auto & index1 = _indices.at(i + 0);
        auto & index2 = _indices.at(i + 1);
        auto & index3 = _indices.at(i + 2);

        const glm::vec3 & pos1 = _positions.at(index1);
        const glm::vec3 & pos2 = _positions.at(index2);
        const glm::vec3 & pos3 = _positions.at(index3);

        const glm::vec2 & uv1 = _uvs.at(index1);
        const glm::vec2 & uv2 = _uvs.at(index2);
        const glm::vec2 & uv3 = _uvs.at(index3);

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;

        if (f == 0.f)
        {
          debug::LogError("MeshProcessor Error: failed to calculate tangent. Matching UVs.");
          continue;
        }

        f = 1.f / f;
        glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * f;
        glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * f;

        for (size_t j = 0u; j < 3u; j++)
        {
          const uint32 & index = _indices[i + j];

          (*_outTangents)[index] += tangent;
          (*_outBitangents)[index] += bitangent;
        }
      }
      
      for (size_t i = 0; i < _positions.size(); i++)
      {
        glm::vec3 & t = (*_outTangents)[i];
        glm::vec3 & b = (*_outBitangents)[i];
        const glm::vec3 & n = _normals[i];

        t = t - n * glm::dot(n, t);
        b = b - n * glm::dot(n, b) - t * glm::dot(t, b);

        if (glm::length(t) > 0) { t = glm::normalize(t); }
        if (glm::length(b) > 0) { b = glm::normalize(b); }
      }
    }
  }
}