#ifndef _ENGINE_GRAPHICS_BASERENDERER_H_
#define _ENGINE_GRAPHICS_BASERENDERER_H_

#include "Renderer.h"

#include "Skybox.h"

namespace engine
{
  namespace graphics
  {
    class BaseRenderer : public Renderer
    {
    protected:
      struct Command
      {
        std::weak_ptr<Mesh> mesh;
        std::weak_ptr<Material> material;
        glm::mat4 transform;
      };
    public:
      virtual ~BaseRenderer();

      virtual void Start(const Camera & _camera);

      virtual void Add(
        std::weak_ptr<Mesh> _mesh,
        std::weak_ptr<Material> _material,
        const glm::mat4 & _transform
      ) override;

      virtual void Add(const Light & _light) override;

      virtual void End();

      virtual void Reset();

      virtual void setSkybox(std::shared_ptr<Skybox> _skybox);
      virtual void setAmbient(const glm::vec3 & _ambient);

    protected:
      BaseRenderer(RenderFlags::Type _flags);

    protected:
      std::vector<Command> m_commands;
      std::vector<Light> m_lights;
      glm::vec3 m_ambient;
      Camera m_camera;

      std::shared_ptr<Skybox> m_skybox;
    };
  }
}

#endif //_ENGINE_GRAPHICS_BASERENDERER_H_