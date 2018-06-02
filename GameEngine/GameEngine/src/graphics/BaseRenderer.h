#ifndef _ENGINE_GRAPHICS_BASERENDERER_H_
#define _ENGINE_GRAPHICS_BASERENDERER_H_

#include "Renderer.h"

#include "Skybox.h"

#include "CommandBuffer.h"

namespace engine
{
  namespace graphics
  {
    class BaseRenderer : public Renderer
    {
    public:
      virtual ~BaseRenderer();

      virtual void Start(const Camera & _camera);

      virtual void Add(
        const std::shared_ptr<Mesh> & _mesh,
        const std::shared_ptr<Material> & _material,
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
      CommandBuffer m_commands;
      std::vector<Light> m_lights;
      glm::vec3 m_ambient;
      Camera m_camera;

      std::shared_ptr<Skybox> m_skybox;
    };
  }
}

#endif //_ENGINE_GRAPHICS_BASERENDERER_H_