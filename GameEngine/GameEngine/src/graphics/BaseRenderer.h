#ifndef _ENGINE_GRAPHICS_BASERENDERER_H_
#define _ENGINE_GRAPHICS_BASERENDERER_H_

#include "Renderer.h"

#include "Skybox.h"

#include "CommandBuffer.h"

namespace engine {
namespace graphics {

  class BaseRenderer : public Renderer
  {
   public:
    virtual ~BaseRenderer();
  
    virtual void Add(
      const std::shared_ptr<Mesh> & _mesh,
      const std::shared_ptr<Material> & _material,
      const glm::mat4 & _transform
    ) override;
  
    virtual void Add(const DirectionalLight & _directional) override;
    virtual void Add(const PointLight & _point) override;
    virtual void Add(const SpotLight & _spot) override;

    virtual void setCamera(const Camera & _camera);

    virtual void End();
  
    virtual void Reset();
  
    virtual void setSkybox(const std::shared_ptr<Skybox> & _skybox);
    virtual void setAmbient(const glm::vec3 & _ambient);

    const CommandBuffer & getCommands() const;
  
   protected:
    BaseRenderer(RenderFlags::Type _flags);
  
   protected:
    CommandBuffer m_commands;
    Lights m_lights;
    Camera m_camera;
  
    std::shared_ptr<Skybox> m_skybox;
  };
 
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_BASERENDERER_H_