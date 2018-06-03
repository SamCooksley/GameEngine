#ifndef _ENGINE_LIGHT_H_
#define _ENGINE_LIGHT_H_

#include "Component.h"
#include "graphics\Light.h"

namespace engine {

  class Light : public Component
  {
   public:
    Light();
    ~Light();

    graphics::Light getLight();

    void setColour(const glm::vec3 & _colour);

    void setDirectional();

    void setPoint(float _linear, float _quadratic);

    void setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic);

   protected:
    void OnAwake() override;
    void OnRender(graphics::Renderer & _renderer) override;

   private:
    graphics::Light m_light;

    float m_cutoffAngle;
    float m_outerCuttofAngle;
  };

} // engine

#endif //_ENGINE_LIGHT_H_
