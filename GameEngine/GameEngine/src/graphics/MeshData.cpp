#include "stdafx.h"

#include "MeshData.h"

namespace engine
{
  namespace graphics
  {
    bool MeshData::HasIndicies() const
    {
      return !indices.empty();
    }

    bool MeshData::HasUVs() const
    {
      return !uvs.empty();
    }

    bool MeshData::HasNormals() const
    {
      return !normals.empty();
    }

    bool MeshData::HasTangents() const
    {
      return !tangents.empty();
    }
  }
}