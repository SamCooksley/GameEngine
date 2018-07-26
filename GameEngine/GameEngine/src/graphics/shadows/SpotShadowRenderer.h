#ifndef _ENGINE_GRAPHICS_SPOTSHADOWRENDERER_H_
#define _ENGINE_GRAPHICS_SPOTSHADOWRENDERER_H_

#include "..\Shader.h"

#include "..\CommandBuffer.h"
#include "..\ShadowMap.h"
#include "..\filter\Blur.h"

namespace engine {
namespace graphics {

  class SpotShadowRenderer
  {
  public:
    SpotShadowRenderer(
      int _size,
      const std::shared_ptr<Shader> & _depth = Shader::Load("resources/shaders/depth.shader"),
      const std::shared_ptr<Filter> & _blur = std::make_shared<Blur>()
    );
    ~SpotShadowRenderer();

    void setLight(
      const glm::vec3 & _position, const glm::vec3 & _direction, 
      float _fov,
      float _near, float _far
    );

    void Render(const CommandBuffer & _commands);

    SpotShadowMap getShadowMap() const;

  private:
    std::shared_ptr<Texture2D> CreateShadowMap();

  private:
    Camera m_camera;

    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<Filter> m_blur;

    std::shared_ptr<FrameBuffer> m_shadowBuffer;

  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_SPOTSHADOWRENDERER_H_