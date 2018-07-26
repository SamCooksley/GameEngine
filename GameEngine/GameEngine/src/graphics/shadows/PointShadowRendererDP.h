#ifndef _ENGINE_GRAPHICS_POINTSHADOWRENDERERDP_H_
#define _ENGINE_GRAPHICS_POINTSHADOWRENDERERDP_H_

#include "..\ShadowMap.h"

#include "..\FrameBuffer.h"

#include "..\Shader.h"

#include "..\CommandBuffer.h"

#include "..\CameraCube.h"
#include "..\filter\BlurArray.h"

namespace engine {
namespace graphics {

  class PointShadowRendererDP
  {
  public:
    PointShadowRendererDP(
      int _size,
      const std::shared_ptr<Shader> & _depth = Shader::Load("resources/shaders/depth_dp.shader"),
      const std::shared_ptr<FilterArray> & _blur = std::make_shared<BlurArray>()
    );
    ~PointShadowRendererDP();

    void setLight(const glm::vec3 & _position, float _near, float _far);

    void Render(const CommandBuffer & _commands);

    DualParaboloidShadowMap getShadowMap() const;

  private:
    std::shared_ptr<Texture2DArray> CreateShadowMap();

  private:
    glm::vec3 m_lightPosition;
    glm::mat4 m_lightSpace;
    float m_near;
    float m_far;

    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<FilterArray> m_blur;

    std::shared_ptr<FrameBuffer> m_shadowBuffer;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_POINTSHADOWRENDERERDP_H_