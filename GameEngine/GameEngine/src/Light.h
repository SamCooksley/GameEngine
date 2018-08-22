#ifndef _ENGINE_LIGHT_H_
#define _ENGINE_LIGHT_H_

#include "Component.h"

#include "graphics\ShadowMap.h"
#include "graphics\ShadowRenderer.h"
#include "graphics\FrameBuffer.h"

#include "Camera.h"

namespace engine {

  enum class LightType
  {
    NONE,
    DIRECTIONAL,
    POINT,
    SPOT
  };

  class Light : public Component
  {
    friend class Application;

   public:
    Light();
    ~Light();

    Light & setColour(const glm::vec3 & _colour);
    Light & setIntensity(float _intensity);

    Light & setDirectional();

    Light & setPoint();

    Light & setSpot(float _cutoff, float _outerCutoff);

    Light & setRadius(float _radius);

    Light & setShadows(bool _castShadows);

   protected:
    void OnAwake() override;
    void OnDestroy() override;
    void OnRender(graphics::Renderer & _renderer) override;

   private:
    LightType m_type;

    glm::vec3 m_colour;
    float m_intensity;

    float m_cutoffAngle;
    float m_outerCutoffAngle;

    float m_radius;

    bool m_castShadows;
    float m_near;

    ENGINE_SETUPSHARED(Light);
  };

} // engine

#endif //_ENGINE_LIGHT_H_
