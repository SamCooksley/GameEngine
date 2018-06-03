#ifndef _ENGINE_GRAPHICS_MESH_PRIMITIVE_H_
#define _ENGINE_GRAPHICS_MESH_PRIMITIVE_H_

#include "../Mesh.h"

namespace engine
{
  namespace graphics
  {
    namespace mesh
    {
      class Primitive
      {
      public:
        const std::shared_ptr<Mesh> & getMesh() const;

      protected:
        std::shared_ptr<Mesh> m_mesh;
      };
    }
  }
}

#endif //_ENGINE_GRAPHICS_MESH_PRIMITIVE_H_
