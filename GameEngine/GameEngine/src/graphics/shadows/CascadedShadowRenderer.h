#ifndef _ENGINE_GRAPHICS_CASCADEDSHADOWRENDERER_H_
#define _ENGINE_GRAPHICS_CASCADEDSHADOWRENDERER_H_

#include "..\Camera.h"

#include "..\ShadowMap.h"

#include "..\filter\BlurArray.h"

#include "..\CommandBuffer.h"

namespace engine {
namespace graphics {

  class CascadedShadowRenderer
  {
  public:
    CascadedShadowRenderer(
      int _size, int _cascadeCount,
      const std::shared_ptr<Shader> & _depth = Shader::Load("resources/shaders/depth_csm.shader"),
      const std::shared_ptr<FilterArray> & _blur = std::make_shared<BlurArray>()
    );
    ~CascadedShadowRenderer();

    void setLight(const glm::vec3 & _direction, const Camera & _camera);

    void Render(const CommandBuffer & _commands);

    const CascadedShadowMap getShadowMap() const;

  private:
    std::shared_ptr<Texture2DArray> CreateVarienceShadowMap();

  private:
    std::vector<Camera> m_cameras;
    std::vector<float> m_distances;

    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<FilterArray> m_blur;

    std::shared_ptr<FrameBuffer> m_shadowBuffer;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_CASCADEDSHADOWRENDERER_H_