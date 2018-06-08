#ifndef _ENGINE_GRAPHICS_LIGHTS_H_
#define _ENGINE_GRAPHICS_LIGHTS_H_

#include "core\Types.h"

#include "Texture2D.h"
#include "TextureCube.h"

namespace engine {
namespace graphics {

  constexpr uint MAX_DIRECTIONAL_LIGHTS = 5u;
  constexpr uint MAX_POINT_LIGHTS = 10u;
  constexpr uint MAX_SPOT_LIGHTS = 5u;

  struct Attenutation
  {
    float linear;
    float quadratic;
  };

  struct DirectionalLight
  {
    glm::vec3 colour;
    glm::vec3 direction;

    glm::vec3 position;
    glm::mat4 lightSpace;
    std::shared_ptr<Texture2D> shadowMap;
  };

  struct PointLight
  {
    glm::vec3 colour;
    glm::vec3 position;

    Attenutation atten;
  };

  struct SpotLight
  {
    glm::vec3 colour;

    glm::vec3 position;
    glm::vec3 direction;

    float cutoff;
    float outerCutoff;

    Attenutation atten;
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