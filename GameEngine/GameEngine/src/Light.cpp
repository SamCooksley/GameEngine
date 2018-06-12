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

        if (m_shadow)
        {
          light.shadowMap = m_shadow;
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
      m_shadow = std::make_shared<graphics::DirectionalShadowMap>();
      m_frameBuffer = graphics::FrameBuffer::Create(1024, 1024);
      m_shadow->shadowMap = m_frameBuffer->AddShadow2D(graphics::TextureFormat::DEPTH_COMPONENT32F);
      m_frameBuffer->Clear();

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
    m_frameBuffer->Clear();

    auto camera = GenerateDirectionalCamera(*_camera);
    m_shadowRenderer->Render(camera, _occluders);
    m_shadow->lightSpace = camera.vp;
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

  graphics::Camera Light::GenerateDirectionalCamera(const Camera & _target)
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
    
    float dist[] = { 0.f, 1.f } ;
    auto points = _target.getFrustumPoints(dist, 2);
    assert(!points.empty());

    glm::vec4 world = cam * glm::vec4(points[0], 1.f);
    glm::vec3 local = glm::vec3(view * world);
    local.z *= -1.f;

    glm::vec3 min(local);
    glm::vec3 max(local);

    for (size_t i = 1u; i < points.size(); ++i)
    {
      world = cam * glm::vec4(points[i], 1.f);
      local = glm::vec3(view * world);
      local.z *= -1.f;

      min = glm::min(min, local);
      max = glm::max(max, local);
    }
    
    glm::mat4 proj = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z);

    return graphics::Camera(proj, view, pos);
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