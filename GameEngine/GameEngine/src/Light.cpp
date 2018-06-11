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

  void Light::OnDestroy()
  {
    RemoveShadow();
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

    UpdateShadow();
  }

  void Light::setPoint(float _linear, float _quadratic)
  {
    m_type = LightType::POINT;

    m_attenuation.linear = _linear;
    m_attenuation.quadratic = _quadratic;

    UpdateShadow();
  }

  void Light::setSpot(float _cutoff, float _outerCutoff, float _linear, float _quadratic)
  {
    m_type = LightType::SPOT;

    m_cutoffAngle = glm::min(_cutoff, 180.f);
    m_outerCuttofAngle = glm::clamp(_outerCutoff, m_cutoffAngle, 180.f);

    m_attenuation.linear = _linear;
    m_attenuation.quadratic = _quadratic;

    UpdateShadow();
  }

  void Light::setShadows(bool _castShadows)
  {
    if (_castShadows)
    {
      //TODO: create shadow;
      m_shadows = std::make_unique<graphics::ShadowMap>(1024, 1024);
      m_castShadows = true;
    }
    else
    {
      m_shadows.reset();
      m_castShadows = false;
    }

    UpdateShadow();
  }

  void Light::GenerateShadowMap(const graphics::ShadowCommandBuffer & _occluders)
  {
    auto trs = getGameObject()->getComponent<Transform>();
    assert(trs);

    glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 25.0f);

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    graphics::Camera camera(proj, view, pos);

    m_shadows->GenerateShadowMap(camera, _occluders);
  }

  graphics::ShadowRenderer * Light::SetupShadowPass()
  {
    auto trs = getGameObject()->getComponent<Transform>();
    assert(trs);

    glm::mat4 proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 25.0f);

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    graphics::Camera cam(proj, view, pos);

    //m_shadows->Setup(cam);

    return nullptr;// m_shadows->getRenderer();
  }

  void Light::UpdateShadow()
  {
    if (m_castShadows)
    {
      AddShadow();
    }
    else
    {
      RemoveShadow();
    }
  }

  void Light::AddShadow()
  {
    if (m_type == m_shadowList) { return; }

    RemoveShadow();

    auto * lights = getShadowList(m_type);
    if (lights == nullptr) { return; }

    lights->push_back(Light::getShared());
    m_shadowList = m_type;
  }

  void Light::RemoveShadow()
  {
    auto * lights = getShadowList(m_shadowList);
    if (lights == nullptr) { return; }

    for (size_t i = 0u; i < lights->size();)
    {
      if ((*lights)[i].get() == this)
      {
        lights->erase(lights->begin() + i);
      }
      else { ++i; }
    }

    m_shadowList = LightType::NONE;
  }

  std::vector<std::shared_ptr<Light>> * Light::getShadowList(LightType _type)
  {
    std::vector<std::shared_ptr<Light>> * list = nullptr;

    switch (_type)
    {
      case engine::LightType::DIRECTIONAL:
      {
        list = &Application::s_context->directionalShadows;
        break;
      }
    }

    return list;
  }

} // engine