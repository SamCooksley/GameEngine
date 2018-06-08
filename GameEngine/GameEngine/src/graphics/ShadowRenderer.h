#ifndef _ENGINE_GRAPHICS_SHADOWRENDERER_H_
#define _ENGINE_GRAPHICS_SHADOWRENDERER_H_

#include "Renderer.h"

#include "CommandBuffer.h"

#include "Resources.h"

namespace engine {
namespace graphics {

  struct ShadowCommand
  {
    std::shared_ptr<Mesh> mesh;
    glm::mat4 transform;
  };

  class ShadowCommandBuffer
  {
  public:
    ShadowCommandBuffer() = default;
    ~ShadowCommandBuffer() = default;

    void Add(
      const std::shared_ptr<Mesh> & _mesh,
      const glm::mat4 & _transform
    );

    void Clear();

    const std::vector<ShadowCommand> & getCommands() const;

   private:
    std::vector<ShadowCommand> m_commands;
  };

  class ShadowRenderer : public Renderer
  {
   public:
    ShadowRenderer(
      const std::shared_ptr<Material> & _material 
                      = Material::Create(Resources::Load<Shader>("resources/shaders/shadow.shader"))
    );
    ~ShadowRenderer();

    void Start(const Camera & _camera);

    void Add(
      const std::shared_ptr<Mesh> & _mesh,
      const std::shared_ptr<Material> & _material,
      const glm::mat4 & _transform
    ) override;

    void End();

    void Render() override;

    void Reset();

  private:
    ShadowCommandBuffer m_commands;
    Camera m_camera;
    std::shared_ptr<Material> m_material;
  };

} } // engine::grahics

#endif //_ENGINE_GRAPHICS_SHADOWRENDERER_H_