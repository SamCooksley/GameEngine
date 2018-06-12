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

    m_shadowCascades = 3;
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

        if (m_shadow)
        {
          light.shadowMaps = m_shadow;
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
      m_shadowRenderer = std::make_shared<graphics::ShadowRenderer>();
      m_frameBuffer = graphics::FrameBuffer::Create(516, 516);//(2048, 2048);
      m_shadow = std::make_shared<graphics::CSM>();

      m_shadow->maps.resize(m_shadowCascades);

      for (auto & cascade : m_shadow->maps)
      {
        cascade.lightSpace = glm::mat4(1.f);
        cascade.distance = 0.f;
        cascade.shadowMap = m_frameBuffer->AddShadow2D(graphics::TextureFormat::DEPTH_COMPONENT32F);
        m_frameBuffer->Clear();
      }

      m_castShadows = true;
    }
    else
    {
      
      m_castShadows = false;
    }

    UpdateShadow();
  }

  void Light::GenerateShadowMap(const graphics::ShadowCommandBuffer & _occluders, const Camera * _camera)
  {
    auto trs = getGameObject()->getComponent<Transform>();
    assert(trs);

    assert(_camera != nullptr);

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    m_frameBuffer->Bind();

    auto camera = GenerateDirectionalCamera(*_camera, m_shadowCascades);

    for (int i = 0u; i < camera.size(); ++i)
    {
      m_frameBuffer->Add(m_shadow->maps[i].shadowMap);
      m_frameBuffer->Clear();
      
      m_shadowRenderer->Render(camera[i], _occluders);

      m_shadow->maps[i].lightSpace = camera[i].vp;
      m_shadow->maps[i].distance = .33333f * (1.f + i) * 10.f;
    }
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

  std::vector<graphics::Camera> Light::GenerateDirectionalCamera(const Camera & _target, size_t _numCascades)
  {
    auto trs = _target.getGameObject()->getComponent<Transform>();
    assert(trs && "Camera does not have transform component");

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 cam = Transform::getTransform(pos, rot, glm::vec3(1.f));

    trs = getGameObject()->getComponent<Transform>();
    assert(trs && "Light does not have transform component");

    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    glm::mat4 mv = view * cam;

    assert(_numCascades > 0 && "Must have atleat one cascade");

    float step = 1.0f / _numCascades;
    std::vector<float> dist(_numCascades + 1u);

    for (size_t i = 0u; i < dist.size(); ++i)
    {
      dist[i] = step * i;
    }

    auto points = _target.getFrustumPoints(dist.data(), dist.size());
    assert(!points.empty());

    std::vector<graphics::Camera> cascades;
    cascades.reserve(_numCascades);
    
    for (size_t i = 0u; i < _numCascades; ++i)
    {
      size_t start = i * 4u;
      glm::vec3 local = glm::vec3(mv * glm::vec4(points[start], 1.f));
      local.z *= -1.f;

      glm::vec3 min(local);
      glm::vec3 max(local);

      for (size_t i = 1u; i < 8u; ++i)
      {
        local = glm::vec3(mv * glm::vec4(points[start + i], 1.f));
        local.z *= -1.f;

        min = glm::min(min, local);
        max = glm::max(max, local);
      }

      //padding
      min -= glm::vec3(5.f);
      max += glm::vec3(5.f);

      glm::mat4 proj = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z);
      cascades.emplace_back(proj, view, pos);
    }

    return cascades;
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