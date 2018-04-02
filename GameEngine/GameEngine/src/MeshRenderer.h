#ifndef _ENGINE_MESHRENDERER_H_
#define _ENGINE_MESHRENDERER_H_

#include "Component.h"

#include "graphics\Mesh.h"
#include "graphics\Material.h"

namespace engine
{
  class MeshRenderer : public Component
  {
  public:
    MeshRenderer();
    ~MeshRenderer();

    void setMesh(std::shared_ptr<graphics::Mesh> _mesh);
    void setMaterial(std::shared_ptr<graphics::Material> _material);

  protected:
    void OnAwake() override;

    void OnRender(graphics::Renderer & _renderer) override;

  private:
    std::shared_ptr<graphics::Mesh> m_mesh;
    std::shared_ptr<graphics::Material> m_material;
  };
}

#endif //_ENGINE_MESHRENDERER_H_