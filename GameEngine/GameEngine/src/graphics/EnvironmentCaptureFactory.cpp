#include "stdafx.h"

#include "EnvironmentCaptureFactory.h"

namespace engine {
namespace graphics {

  EnvironmentCaptureFactory::EnvironmentCaptureFactory(
    const std::shared_ptr<FilterCube> & _irradanceFilter,
    const std::shared_ptr<FilterCube> & _prefilter
  ) :
    m_irradanceFilter(_irradanceFilter),
    m_prefilter(_prefilter)
  { }

  EnvironmentCaptureFactory::~EnvironmentCaptureFactory()
  { }

  EnvironmentCapture EnvironmentCaptureFactory::Create(TextureCube & _src)
  {
    EnvironmentCapture capture;

    capture.irradiance = std::make_shared<TextureCube>(TextureFormat::RGB16F, 32, 32, 1);

    m_irradanceFilter->Apply(_src, *capture.irradiance);

    capture.prefiltered = std::make_shared<TextureCube>(TextureFormat::RGB16F, 128, 128, 0);

    for (int i = 0; i < capture.prefiltered->getMipmapCount(); ++i)
    {
      float roughness = float(i) / float(capture.prefiltered->getMipmapCount() - 1);
      m_prefilter->setUniform("roughness", roughness);

      TextureCube tmp(*capture.prefiltered, i);

      m_prefilter->Apply(_src, tmp);
    }

    return capture;
  }

} } // engine::graphics