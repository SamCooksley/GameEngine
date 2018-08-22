#ifndef _ENGINE_GRAPHICS_ENVIRONMENTCAPTUREFACTORY_H_
#define _ENGINE_GRAPHICS_ENVIRONMENTCAPTUREFACTORY_H_

#include "EnvironmentCapture.h"

#include "filter\FilterCube.h"

namespace engine {
namespace graphics {

  class EnvironmentCaptureFactory
  {
  public:
    EnvironmentCaptureFactory(
      const std::shared_ptr<FilterCube> & _irradanceFilter = std::make_shared<FilterCube>(Shader::Load("resources/shaders/pbr/ibl/irradiance.glsl")),
      const std::shared_ptr<FilterCube> & _prefilter       = std::make_shared<FilterCube>(Shader::Load("resources/shaders/pbr/ibl/prefiltered.glsl"))
    );
    ~EnvironmentCaptureFactory();

    EnvironmentCapture Create(TextureCube & _src);

  private:
    std::shared_ptr<FilterCube> m_irradanceFilter;
    std::shared_ptr<FilterCube> m_prefilter;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_ENVIRONMENTCAPTUREFACTORY_H_