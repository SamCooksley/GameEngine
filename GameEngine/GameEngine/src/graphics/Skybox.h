#ifndef _ENGINE_GRAPHICS_SKYBOX_H_
#define _ENGINE_GRAPHICS_SKYBOX_H_

#include "Mesh.h"
#include "Material.h"

#include "Graphics_Camera.h"

#include "mesh\mesh_skybox.h"

namespace engine
{
  namespace graphics
  {
    class Skybox : public Object
    {
    public:
      Skybox(
        std::shared_ptr<Material> _material,
        std::shared_ptr<Mesh> _mesh = mesh::Skybox().getMesh()
      );
      ~Skybox();
      
      void Render(Camera & _camera);
      void Render();

    private:
      std::shared_ptr<Material> m_material;
      std::shared_ptr<Mesh> m_mesh;
    };
  }
}

#endif //_ENGINE_GRAPHICS_SKYBOX_H_