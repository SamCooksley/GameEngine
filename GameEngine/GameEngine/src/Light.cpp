#include "stdafx.h"

#include "Light.h"

#include "GameObject.h"

#include "Application.h"
#include "Resources.h"

#include "Input.h"

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

    m_depth = Resources::Load<graphics::Shader>("resources/shaders/depth_csm_varience.shader");
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
          light.shadow = m_shadow;
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
      uint size = 512;//2048;//1024;

      //m_shadowRenderer = std::make_shared<graphics::ShadowRenderer>();
      m_frameBuffer = graphics::FrameBuffer::Create(size, size);
      m_frameBuffer->setClearColour(glm::vec4(1.f, 0.f, 0.f, 0.f));

      //create a depth buffer to keep depth testing.
      //render buffers cannot be layered so use a texture.
      //use shadow2D for 
      auto depth = std::make_shared<graphics::Shadow2DArray>(size, size, 4, graphics::TextureFormat::DEPTH_COMPONENT32F);
      m_frameBuffer->Add(depth);
    
      m_shadow = std::make_shared<graphics::CSM>();
      
      m_shadow->distance.resize(4);
      m_shadow->lightSpace.resize(4);
      
      auto colour = std::make_shared<graphics::Texture2DArray>(size, size, 4, 1, graphics::TextureFormat::RG32F);
      colour->setFilter(graphics::TextureFilter::LINEAR);
      colour->setWrap(graphics::TextureWrap::CLAMP_TO_BORDER);
      colour->setBorder(glm::vec4(1.f));
      m_frameBuffer->Add(colour, graphics::FrameBufferAttachment::COLOUR);

      m_shadow->shadowMap = colour;

      m_frameBuffer->Clear();

      m_blur = graphics::Shader::Load("resources/shaders/filters/blur_array4.shader");

      for (int i = 0; i < 2; ++i)
      {
        m_pingpong[i] = graphics::FrameBuffer::Create(size, size);
        auto tex = std::make_shared<graphics::Texture2DArray>(size, size, 4, 1, graphics::TextureFormat::RG32F);
        
        tex->setWrap(graphics::TextureWrap::CLAMP_TO_BORDER);
        tex->setBorder(glm::vec4(1.f));
        m_pingpong[i]->Add(tex, graphics::FrameBufferAttachment::COLOUR);
      }

      m_castShadows = true;
    }
    else
    {
      m_castShadows = false;
    }

    UpdateShadow();
  }

  void Light::GenerateShadowMap(const graphics::ShadowCommandBuffer & _occluders, const graphics::Camera * _camera)
  {
    auto trs = getGameObject()->getComponent<Transform>();
    assert(trs);

    assert(_camera != nullptr);

    glm::vec3 pos; glm::quat rot;
    trs->get(&pos, &rot, nullptr);
    glm::mat4 view = Transform::getTransform(pos, rot, glm::vec3(1.f));
    view = glm::inverse(view);

    auto camera = GenerateDirectionalCamera(*_camera, 4);

    m_frameBuffer->Bind();
    m_frameBuffer->Clear();
    m_depth->Bind();
    for (int i = 0; i < 4; ++i)
    {
      m_depth->setUniform("vp[" + std::to_string(i) + ']', camera[i].camera.vp);

      m_shadow->lightSpace[i] = camera[i].camera.vp;
      m_shadow->distance[i] = camera[i].distance;
    }

    const auto & commands = _occluders.getCommands();
    for (auto & command : commands)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }
    //return;

    m_blur->Bind();
    m_blur->setUniform("tex", 0);


    int passes = 2;
    passes = std::max(passes, 2);

    auto start = m_frameBuffer->getColour<graphics::Texture2DArray>(0);

    auto target = m_frameBuffer;

    bool ping = false;

    for (int i = 0; i < passes; ++i)
    {
      if (i == 0)
      {
        start->Bind(0);
      }
      else
      {
        m_pingpong[!ping]->getColour<graphics::Texture2DArray>(0)->Bind(0);
      }

      if (i == passes - 1)
      {
        target->Bind();
        target->Clear();
      }
      else
      {
        m_pingpong[ping]->Bind();
        m_pingpong[ping]->Clear();
      }

      if (i % 2 == 0)
      {
        m_blur->setUniform("scale", glm::vec2(1, 0));
      }
      else
      {
        m_blur->setUniform("scale", glm::vec2(0, 1));
      }

      target->RenderToNDC();

      ping = !ping;
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

  std::vector<LightCameraStep> Light::GenerateDirectionalCamera(const graphics::Camera & _target, size_t _numCascades)
  {
    auto transform = getGameObject()->getComponent<Transform>();
    assert(transform && "Light does not have a transform component");

    glm::vec3 direction = glm::normalize(transform->getForward());

    glm::vec3 up(0.f, 1.f, 0.f);
    glm::vec3 xaxis = glm::cross(up, direction);
    xaxis = glm::normalize(xaxis);

    glm::vec3 yaxis = glm::cross(direction, xaxis);
    yaxis = glm::normalize(yaxis);

    glm::mat4 model(1.f);
    model[0] = glm::vec4(xaxis, 0.f);
    model[1] = glm::vec4(yaxis, 0.f);
    model[2] = glm::vec4(direction, 0.f);

    glm::mat4 cam = glm::inverse(_target.view);

    assert(_numCascades > 0 && "Must have at least one cascade");

    std::vector<float> dist(_numCascades + 1u);
    float weight = .5f;


    for (int i = 0u; i < dist.size(); ++i)
    {
      float no = static_cast<float>(i);
      float uniform = glm::mix(_target.near, _target.far, no / _numCascades);
      float near = glm::max(_target.near, .000001f);
      float log = near * glm::pow((_target.far / near), no / _numCascades);
      dist[i] = glm::mix(uniform, log, weight);
    }

    auto points = _target.getFrustumPoints(dist.data(), dist.size());
    assert(!points.empty());

    std::vector<LightCameraStep> cascades(_numCascades);

    for (size_t i = 0u; i < _numCascades; ++i)
    {
      size_t start = i * 4u;
      glm::vec3 world = glm::vec3(cam * glm::vec4(points[start], 1.f));

      glm::vec3 min(world);
      glm::vec3 max(world);

      for (size_t i = 1u; i < 8u; ++i)
      {
        world = glm::vec3(cam * glm::vec4(points[start + i], 1.f));

        min = glm::min(min, world);
        max = glm::max(max, world);
      }

      glm::vec3 avg = (min + max) * .5f;

      glm::vec3 halfSize = max - avg;

      std::array<glm::vec3, 4> axes = {
          glm::vec3(halfSize.x,  halfSize.y,  halfSize.z),
          glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
          glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
          glm::vec3(halfSize.x, -halfSize.y,  halfSize.z)
      };
      
      float height = glm::max(
                      glm::abs(glm::dot(axes[0], yaxis)),
                      glm::abs(glm::dot(axes[1], yaxis)),
                      glm::abs(glm::dot(axes[2], yaxis)),
                      glm::abs(glm::dot(axes[3], yaxis))
                     );

      float width = glm::max(
                     glm::abs(glm::dot(axes[0], xaxis)),
                     glm::abs(glm::dot(axes[1], xaxis)),
                     glm::abs(glm::dot(axes[2], xaxis)),
                     glm::abs(glm::dot(axes[3], xaxis))
                    );

      float aspect = width / height;

      float depth = glm::max(
                     glm::abs(glm::dot(axes[0], direction)),
                     glm::abs(glm::dot(axes[1], direction)),
                     glm::abs(glm::dot(axes[2], direction)),
                     glm::abs(glm::dot(axes[3], direction))
                    );

      
      //additional padding to get shadows of objects out of the bounds.
      float padding = 10.f;

      model[3] = glm::vec4(avg, 1.f);
      glm::mat4 view = glm::inverse(model);

      cascades[i].camera = graphics::Camera(
        graphics::CameraType::ORTHOGRAPHIC,
        height, aspect, -depth - padding, depth,
        view, avg
      );

      cascades[i].distance = dist[1 + i];
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