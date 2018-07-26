#ifndef _ENGINE_GRAPHICS_SHADOWMAP_H_
#define _ENGINE_GRAPHICS_SHADOWMAP_H_

#include "Shadow2D.h"
#include "Shadow2DArray.h"
#include "ShadowCube.h"

#include "Texture2D.h"
#include "Texture2DArray.h"
#include "TextureCube.h"

namespace engine { 
namespace graphics {
 
  struct CascadedShadowMap
  {
   public:
    std::vector<float> distances;
    std::vector<glm::mat4> lightSpaces;
    std::shared_ptr<Texture2DArray> shadowMap;
  };

  struct DualParaboloidShadowMap
  {
   public:
    glm::mat4 lightSpace;
    float near;
    float far;
    std::shared_ptr<Texture2DArray> shadowMap;
  };

  struct CubeShadowMap
  {
   public:
    float near;
    float far;
    std::shared_ptr<TextureCube> shadowMap;
  };

  struct SpotShadowMap
  {
   public:
    glm::mat4 lightSpace;
    std::shared_ptr<Texture2D> shadowMap;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOWMAP_H_