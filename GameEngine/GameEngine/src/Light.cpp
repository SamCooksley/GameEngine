#include "stdafx.h"

#include "Light.h"

#include "GameObject.h"

namespace engine
{
  Light::Light()
  { }

  Light::~Light()
  { }

  void Light::OnAwake()
  {
    Component::OnAwake();

    setColour(glm::vec3(1.f));
    setDirectional();
  }

  void Light::OnRender(graphics::Renderer & _renderer)
  {
    Component::OnRender(_renderer);

    _renderer.Add(getLight());
  }

  graphics::Light Light::getLight()
  {
    auto transform = getGameObject()->getComponent<Transform>();

    glm::vec3 pos; glm::quat rot;
    transform->get(&pos, &rot, nullptr);

    glm::mat4 trs = Transform::getTransform(pos, rot, glm::vec3(1.0f));

    m_light.position = pos;
    m_light.direction = Transform::getForward(trs);

    return m_light;
  }

  void Light::setColour(const glm::vec3 & _colour)
  {
    m_light.colour = _colour;
  }

  void Light::setDirectional()
  {
    m_light.type = graphics::LightType::Directional;
  }

  void Light::setPoint(float _linear, float _quadratic)
  {
    m_light.type = graphics::LightType::Point;

    m_light.linear = _linear;
    m_light.quadratic = _quadratic;
  }

  void Light::setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic)
  {
    m_light.type = graphics::LightType::Spot;

    m_cutoffAngle = glm::min(_cutoff, 180.f);
    m_outerCuttofAngle = glm::clamp(_outerCutoff, m_cutoffAngle, 180.f);

    m_light.cutoff = glm::cos(glm::radians(m_cutoffAngle));
    m_light.outerCutoff = glm::cos(glm::radians(m_outerCuttofAngle));

    m_light.linear = _linear;
    m_light.quadratic = _quadratic;
  }
}