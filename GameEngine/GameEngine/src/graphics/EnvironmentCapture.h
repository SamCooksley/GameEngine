#ifndef _ENGINE_GRAPHICS_ENVIRONMENTCAPTURE_H_
#define _ENGINE_GRAPHICS_ENVIRONMENTCAPTURE_H_

#include "TextureCube.h"

namespace engine {
namespace graphics {

  struct EnvironmentCapture
  {
  public:
    std::shared_ptr<TextureCube> irradiance;
    std::shared_ptr<TextureCube> prefiltered;

    glm::vec3 position;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_ENVIRONMENTCAPTURE_H_