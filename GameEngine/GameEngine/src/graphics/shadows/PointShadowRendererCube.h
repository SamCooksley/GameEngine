#ifndef _ENGINE_GRAPHICS_POINTSHADOWRENDERCUBE_H_
#define _ENGINE_GRAPHICS_POINTSHADOWRENDERCUBE_H_

#include "..\Shader.h"

#include "..\CommandBuffer.h"

#include "..\CameraCube.h"

#include "..\FrameBuffer.h"

#include "..\filter\BlurArray.h"

namespace engine {
namespace graphics {

  class PointShadowRendererCube
  {
  public:
    PointShadowRendererCube(
      int _size,
      const std::shared_ptr<Shader> & _depth = Shader::Load("resources/shaders/depth_cube.shader")
    );
    ~PointShadowRendererCube();

    void setLight(const glm::vec3 & _position, float _near, float _far);

    void Render(const CommandBuffer & _commands);

    CubeShadowMap getShadowMap() const;

  private:
    std::shared_ptr<TextureCube> CreateShadowMap();

  private:
    CameraCube m_camera;

    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<FilterArray> m_blur;

    std::shared_ptr<FrameBuffer> m_shadowBuffer;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_POINTSHADOWRENDERCUBE_H_
