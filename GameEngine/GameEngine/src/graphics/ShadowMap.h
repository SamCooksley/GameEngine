#ifndef _ENGINE_GRAPHICS_SHADOWMAP_H_
#define _ENGINE_GRAPHICS_SHADOWMAP_H_

#include "Shadow2D.h"
#include "Shadow2DArray.h"
#include "Texture2D.h"
#include "Texture2DArray.h"

namespace engine { 
namespace graphics {
 
  struct DirectionalShadowMap
  {
   public:
    float distance;
    glm::mat4 lightSpace;
    std::shared_ptr<Shadow2D> shadowMap;
  };

  struct CSM
  {
  public:
    std::vector<float> distance;
    std::vector<glm::mat4> lightSpace;
    std::shared_ptr<Texture2DArray> shadowMap;
  };

  /*
  public:
    ShadowMap(
      uint _width, uint _height,
      TextureFormat _format = TextureFormat::DEPTH_COMPONENT32F, 
      const std::shared_ptr<Shader> & _depth = nullptr
    );
    ~ShadowMap();

    const glm::mat4 & getLightSpace() const;

    const std::shared_ptr<Shadow2D> & getShadowMap() const;

    void GenerateShadowMap(const Camera & _camera, const ShadowCommandBuffer & _occluders);

  private:
    glm::mat4 m_lightSpace;
    std::shared_ptr<Shader> m_depth;
    std::shared_ptr<FrameBuffer> m_frameBuffer;
    std::shared_ptr<Shadow2D> m_shadowMap;
  };
  */

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOWMAPPING_H_