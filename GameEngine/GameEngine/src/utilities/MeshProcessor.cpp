#include "stdafx.h"

#include "MeshProcessor.h"

#include "debug\Debug.h"

namespace engine {
namespace utilities {

  MeshProcessor::MeshProcessor()
  { }
  
  MeshProcessor::~MeshProcessor()
  { }
  
  MeshProcessor::Error MeshProcessor::CalculateNormals(
    const graphics::Indices & _indices,
    const std::vector<glm::vec3> & _positions,
    std::vector<glm::vec3> * _outNormals
  ) const
  {
    _outNormals->resize(_positions.size());
  
    std::array<glm::vec3, 3> pos;
    for (size_t i = 0; i < _indices.size(); i += 3)
    {
      for (size_t j = 0u; j < 3u; ++j)
      {
        pos[j] = _positions[_indices[i + j]];
      }
  
      glm::vec3 normal = glm::normalize(glm::cross(pos[1] - pos[0], pos[2] - pos[0]));
  
      for (size_t j = 0u; j < 3u; ++j)
      {
        (*_outNormals)[_indices[i + j]] += normal;
      }
    }
  
    for (auto & normal : *_outNormals)
    {
      if (glm::length2(normal) > 0.f)
      {
        normal = glm::normalize(normal);
      }
    }
  
    return Error::OK;
  }
  
  MeshProcessor::Error MeshProcessor::CalculateTangents(
    const graphics::Indices & _indices,
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
  
    Error error = Error::OK;
  
    for (size_t i = 0u; i < _indices.size(); i += 3u)
    {
      std::array<glm::vec3, 3u> pos;
      std::array<glm::vec2, 3u> uv;
  
      for (size_t j = 0u; j < 3u; ++j)
      {
        auto index = _indices[i + j];
        pos[j] = _positions[index];
        uv[j] = _uvs[index];
      }
  
      std::array<glm::vec3, 2> edge = {
        pos[1] - pos[0],
        pos[2] - pos[0]
      };
  
      std::array<glm::vec2, 2> deltaUV = {
        uv[1] - uv[0],
        uv[2] - uv[0]
      };
  
      float f = deltaUV[0].x * deltaUV[1].y - deltaUV[0].y * deltaUV[1].x;
  
      glm::vec3 tangent;
      glm::vec3 bitangent;
  
      if (f == 0.f)
      {
        error = Error::MATCHING_UVS;
  
        tangent = edge[0];
        bitangent = edge[1];
      }
      else
      {
        f = 1.f / f;
        tangent   = (edge[0] * deltaUV[1].y - edge[1] * deltaUV[0].y) * f;
        bitangent = (edge[1] * deltaUV[0].x - edge[0] * deltaUV[1].x) * f;
      }
  
      for (size_t j = 0u; j < 3u; j++)
      {
        auto index = _indices[i + j];
  
        (*_outTangents)[index] += tangent;
        (*_outBitangents)[index] += bitangent;
      }
    }
    
    for (size_t i = 0u; i < _positions.size(); i++)
    {
      glm::vec3 & t = (*_outTangents)[i];
      glm::vec3 & b = (*_outBitangents)[i];
      const glm::vec3 & n = _normals[i];
  
      t = t - n * glm::dot(n, t);
      b = b - n * glm::dot(n, b) - t * glm::dot(t, b);
  
      if (glm::length2(t) > 0.f) { t = glm::normalize(t); }
      if (glm::length2(b) > 0.f) { b = glm::normalize(b); }
    }
  
    return error;
  }

} } // engine::utilities