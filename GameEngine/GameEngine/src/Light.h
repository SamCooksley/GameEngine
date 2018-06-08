#ifndef _ENGINE_LIGHT_H_
#define _ENGINE_LIGHT_H_

#include "Component.h"

namespace engine {

  enum class LightType
  {
    DIRECTIONAL,
    POINT,
    SPOT
  };

  class Light : public Component
  {
   public:
    Light();
    ~Light();

    void setColour(const glm::vec3 & _colour);

    void setDirectional();

    void setPoint(float _linear, float _quadratic);

    void setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic);

    void setShadows(bool _castShadows);

   protected:
    void OnAwake() override;
    void OnRender(graphics::Renderer & _renderer) override;

   private:
    void AddShadow();
    void RemoveShadow();

   private:
    LightType m_type;

    glm::vec3 m_colour;
    float m_intensity;

    float m_cutoffAngle;
    float m_outerCuttofAngle;

    graphics::Attenutation m_attenuation;

    ENGINE_SETUPSHARED(Light);
  };

} // engine

#endif //_ENGINE_LIGHT_H_
