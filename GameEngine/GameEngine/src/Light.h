#ifndef _ENGINE_LIGHT_H_
#define _ENGINE_LIGHT_H_

#include "Component.h"

#include "graphics\ShadowMap.h"
#include "graphics\ShadowRenderer.h"

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

    void setColour(const glm::vec3 & _colour);

    void setDirectional();

    void setPoint(float _linear, float _quadratic);

    void setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic);

    void setShadows(bool _castShadows);

    void GenerateShadowMap(const graphics::ShadowCommandBuffer & _occluders, const Camera * _camera);

   protected:
    void OnAwake() override;
    void OnDestroy() override;
    void OnRender(graphics::Renderer & _renderer) override;

   private:
    void UpdateShadow();

    std::vector<graphics::Camera> GenerateDirectionalCamera(const Camera & _target, size_t _numCascades);

    void AddShadow();
    void RemoveShadow();

    std::vector<std::shared_ptr<Light>> * getShadowList(LightType _type);

   private:
    LightType m_type;

    glm::vec3 m_colour;
    float m_intensity;

    float m_cutoffAngle;
    float m_outerCuttofAngle;

    graphics::Attenutation m_attenuation;

    bool m_castShadows;
    LightType m_shadowList;

    int m_shadowCascades;

    std::shared_ptr<graphics::FrameBuffer> m_frameBuffer;
    std::shared_ptr<graphics::ShadowRenderer> m_shadowRenderer;
    //union with other shadow types
    std::shared_ptr<graphics::CSM> m_shadow;

    ENGINE_SETUPSHARED(Light);
  };

} // engine

#endif //_ENGINE_LIGHT_H_
