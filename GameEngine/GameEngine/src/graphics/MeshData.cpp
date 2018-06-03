#include "stdafx.h"

#include "MeshData.h"

namespace engine {
namespace graphics {

  bool MeshData::HasIndicies() const
  {
    return !indices.empty();
  }
  
  bool MeshData::HasUVs() const
  {
    return uvs.size() == positions.size();
  }
  
  bool MeshData::HasNormals() const
  {
    return normals.size() == positions.size();
  }
  
  bool MeshData::HasTangents() const
  {
    return tangents.size() == positions.size() && bitangents.size() == positions.size();
  }

} } // engine::graphics