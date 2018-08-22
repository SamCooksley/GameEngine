#include "stdafx.h"

#include "Light.h"

#include "GameObject.h"

#include "Application.h"
#include "Resources.h"

#include "Input.h"

namespace engine {
  
  Light::Light() :
    m_type(LightType::NONE),
    m_colour(0.f), m_intensity(0.f),
    m_cutoffAngle(0.f), m_outerCutoffAngle(0.f),
    m_radius(0.f),
    m_castShadows(false),
    m_near(0.f)
  { }

  Light::~Light()
  { }

  void Light::OnAwake()
  {
    Component::OnAwake();

    setDirectional();
    setColour(glm::vec3(1.f));
    m_intensity = 1.f;

    m_radius = 1.f;

    m_near = .6f;
  }

  void Light::OnDestroy()
  { }

  void Light::OnRender(graphics::Renderer & _renderer)
  {
    Component::OnRender(_renderer);

    auto transform = getGameObject()->getComponent<Transform>();
    assert(transform);

    glm::vec3 pos; glm::quat rot;
    transform->get(&pos, &rot, nullptr);

    glm::mat4 trs = Transform::getTransform(pos, rot, glm::vec3(1.f));
    glm::vec3 dir = Transform::getForward(trs);

    switch (m_type)
    {
      case LightType::DIRECTIONAL:
      {
        graphics::DirectionalLight light;
        light.colour = m_colour * m_intensity;
        light.direction = dir;

        light.shadowData.enable = m_castShadows;
        if (m_castShadows)
        {
          light.shadowData.near = 0.f;
          light.shadowData.far = 0.f;
        }

        _renderer.Add(light);
        break;
      }
      case LightType::POINT:
      {
        graphics::PointLight point;
        point.colour = m_colour * m_intensity;
        point.position = pos;
        point.radius = m_radius;

        point.shadowData.enable = m_castShadows;
        if (m_castShadows)
        {
          point.shadowData.near = m_near;
          point.shadowData.far = m_radius;
        }

        _renderer.Add(point);
        break;
      }
      case LightType::SPOT:
      {
        graphics::SpotLight spot;
        spot.colour = m_colour * m_intensity;
        spot.position = pos;
        spot.direction = dir;
        spot.cutoff = glm::cos(glm::radians(m_cutoffAngle * .5f));
        spot.outerCutoff = glm::cos(glm::radians(m_outerCutoffAngle * .5f));
        spot.fov = m_outerCutoffAngle;
        spot.radius = m_radius;

        spot.shadowData.enable = m_castShadows;
        if (m_castShadows)
        {
          spot.shadowData.near = m_near;
          spot.shadowData.far = m_radius;
        }

        _renderer.Add(spot);
        break;
      }
    }
  }

  Light & Light::setColour(const glm::vec3 & _colour)
  {
    m_colour = _colour;

    return *this;
  }

  Light & Light::setIntensity(float _intensity)
  {
    m_intensity = _intensity;

    return *this;
  }

  Light & Light::setDirectional()
  {
    m_type = LightType::DIRECTIONAL;

    return *this;
  }

  Light & Light::setPoint()
  {
    m_type = LightType::POINT;

    return *this;
  }

  Light & Light::setSpot(float _cutoff, float _outerCutoff)
  {
    m_type = LightType::SPOT;

    m_cutoffAngle = glm::clamp(_cutoff, 0.f, 179.f);
    m_outerCutoffAngle = glm::clamp(_outerCutoff, m_cutoffAngle, 179.f);

    return *this;
  }

  Light & Light::setRadius(float _radius)
  {
    m_radius = std::max(_radius, 0.f);

    return *this;
  }

  Light & Light::setShadows(bool _castShadows)
  {
    m_castShadows = _castShadows;

    return *this;
  }

} // engine