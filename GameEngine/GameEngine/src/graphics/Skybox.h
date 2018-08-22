#ifndef _ENGINE_GRAPHICS_SKYBOX_H_
#define _ENGINE_GRAPHICS_SKYBOX_H_

#include "Mesh.h"
#include "Material.h"

#include "Camera.h"

#include "mesh\skybox.h"

namespace engine {
namespace graphics {

  class Skybox 
  {
   public:
    Skybox(
      const std::shared_ptr<Material> & _material,
      const std::shared_ptr<Mesh> & _mesh = mesh::Skybox().getMesh()
    );
    ~Skybox();
    
    void Render(const Camera & _camera);

    std::shared_ptr<TextureCube> getTexture() const;
    void setTexture(const std::shared_ptr<TextureCube> & _texture);
  
   private:
    std::shared_ptr<Material> m_material;
    std::shared_ptr<Mesh> m_mesh;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SKYBOX_H_