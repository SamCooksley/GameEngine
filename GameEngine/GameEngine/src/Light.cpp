#include "stdafx.h"

#include "Light.h"

#include "GameObject.h"

#include "Application.h"

namespace engine {

  Light::Light()
  { }

  Light::~Light()
  { }

  void Light::OnAwake()
  {
    Component::OnAwake();

    setColour(glm::vec3(1.f));
    setDirectional();

    m_intensity = 1.0f;
  }

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

        if (m_shadows)
        {
          light.lightSpace = m_shadows->getLightSpace();
          light.shadowMap = m_shadows->getShadowMap();
        }

        _renderer.Add(light);
        break;
      }
      case LightType::POINT:
      {
        graphics::PointLight point;
        point.colour = m_colour * m_intensity;
        point.position = pos;
        point.atten.linear = m_attenuation.linear;
        point.atten.quadratic = m_attenuation.quadratic;
        _renderer.Add(point);
        break;
      }
      case LightType::SPOT:
      {
        graphics::SpotLight spot;
        spot.colour = m_colour * m_intensity;
        spot.position = pos;
        spot.direction = dir;
        spot.cutoff = glm::cos(glm::radians(m_cutoffAngle));
        spot.outerCutoff = glm::cos(glm::radians(m_outerCuttofAngle));
        spot.atten.linear = m_attenuation.linear;
        spot.atten.quadratic = m_attenuation.quadratic;
        _renderer.Add(spot);
        break;
      }
    }
  }

  void Light::setColour(const glm::vec3 & _colour)
  {
    m_colour = _colour;
  }

  void Light::setDirectional()
  {
    m_type = LightType::DIRECTIONAL;
  }

  void Light::setPoint(float _linear, float _quadratic)
  {
    m_type = LightType::POINT;

    m_attenuation.linear = _linear;
    m_attenuation.quadratic = _quadratic;
  }

  void Light::setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic)
  {
    m_type = LightType::SPOT;

    m_cutoffAngle = glm::min(_cutoff, 180.f);
    m_outerCuttofAngle = glm::clamp(_outerCutoff, m_cutoffAngle, 180.f);

    m_attenuation.linear = _linear;
    m_attenuation.quadratic = _quadratic;
  }

  void Light::setShadows(bool _castShadows)
  {
    if (_castShadows)
    {
      //TODO: create shadow;
      m_shadows = std::make_unique<graphics::ShadowMapping>(2048, 2048);
      AddShadow();
    }
    else
    {
      RemoveShadow();
    }
  }

  graphics::ShadowMapping * Light::getShadow()
  {
    return m_shadows.get();
  }

  void Light::SetupShadowPass()
  {
    auto trs = getGameObject()->getComponent<Transform>();
    assert(trs);

    glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    graphics::Camera cam(proj, view, pos);

    m_shadows->Setup(cam);
  }

  void Light::AddShadow()
  {
    auto & lights = Application::s_context->shadowLights;

    for (auto & light : lights)
    {
      if (light.lock().get() == this)
      {
        return;
      }
    }

    lights.push_back(Light::getShared());
  }

  void Light::RemoveShadow()
  {
    auto & lights = Application::s_context->shadowLights;

    for (size_t i = 0u; i < lights.size();)
    {
      if (lights[i].expired() ||
        lights[i].lock().get() == this)
      {
        lights.erase(lights.begin() + i);
      }
      else { ++i; }
    }
  }

} // engine