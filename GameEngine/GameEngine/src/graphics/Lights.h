#ifndef _ENGINE_GRAPHICS_LIGHTS_H_
#define _ENGINE_GRAPHICS_LIGHTS_H_

#include "core\Types.h"

#include "ShadowMap.h"

namespace engine {
namespace graphics {

  constexpr size_t MAX_DIRECTIONAL_CASCADES = 10u;

  struct ShadowData
  {
    bool enable;
    float near;
    float far;
  };

  struct DirectionalLight
  {
    glm::vec3 colour;
    glm::vec3 direction;

    ShadowData shadowData;
    CascadedShadowMap shadow;
  };

  struct PointLight
  {
    glm::vec3 colour;
    glm::vec3 position;

    float radius;

    ShadowData shadowData;
    CubeShadowMap shadow;
  };

  struct SpotLight
  {
    glm::vec3 colour;

    glm::vec3 position;
    glm::vec3 direction;

    float cutoff;
    float outerCutoff;

    float fov;

    float radius;

    ShadowData shadowData;
    SpotShadowMap shadow;
  };

  struct Lights
  {
    glm::vec3 ambient;

    std::vector<DirectionalLight> directional;
    std::vector<PointLight> point;
    std::vector<SpotLight> spot;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_LIGHTS_H_