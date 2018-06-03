#ifndef _ENGINE_GRAPHICS_SKYBOX_H_
#define _ENGINE_GRAPHICS_SKYBOX_H_

#include "Mesh.h"
#include "Material.h"

#include "Camera.h"

#include "mesh\skybox.h"

namespace engine
{
  namespace graphics
  {
    class Skybox 
    {
    public:
      Skybox(
        std::shared_ptr<Material> _material,
        std::shared_ptr<Mesh> _mesh = mesh::Skybox().getMesh()
      );
      ~Skybox();
      
      void Render(const Camera & _camera);

    private:
      std::shared_ptr<Material> m_material;
      std::shared_ptr<Mesh> m_mesh;
    };
  }
}

#endif //_ENGINE_GRAPHICS_SKYBOX_H_