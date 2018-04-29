#include "stdafx.h"

#include "mesh_primitive.h"

namespace engine
{
  namespace graphics
  {
    namespace mesh
    {
      std::shared_ptr<Mesh> Primitive::getMesh() const
      {
        return m_mesh;
      }
    }
  }
}