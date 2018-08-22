#include "stdafx.h"

#include "CascadedShadowRenderer.h"

#include "Input.h"

namespace engine {
namespace graphics {

  CascadedShadowRenderer::CascadedShadowRenderer(
    int _size, int _cascadeCount,
    const std::shared_ptr<Shader> & _depth,
    const std::shared_ptr<FilterArray> & _blur
  ) :
    m_distances(_cascadeCount), m_vps(_cascadeCount),
    m_depth(_depth),
    m_blur(_blur),
    m_shadowBuffer(FrameBuffer::Create(_size, _size, _cascadeCount))
  {
    if (!m_depth)
    {
      throw std::invalid_argument("null depth shader");
    }

    m_shadowBuffer->AttachDepth(
      std::make_shared<Shadow2DArray>(TextureFormat::DEPTH_COMPONENT32F, _size, _size, _cascadeCount)
    );
    m_shadowBuffer->setClearColour(glm::vec4(1.f));
  }

  CascadedShadowRenderer::~CascadedShadowRenderer()
  { }

  void CascadedShadowRenderer::setLight(const glm::vec3 & _direction, const Camera & _camera)
  {
    size_t cascadeCount = m_vps.size();

    m_shadowBuffer->Bind();
    m_shadowBuffer->Attach(CreateVarienceShadowMap(), FrameBufferAttachment::COLOUR, 0);
    m_shadowBuffer->Clear();

    glm::mat4 cam = glm::inverse(_camera.view);

    glm::vec3 up(0.f, 1.f, 0.f);
    {
      glm::vec3 right = glm::cross(up, _direction);
      up = glm::cross(_direction, right);
    }

    std::vector<float> dist(cascadeCount + 1);

    float weight = .5f;

    for (size_t i = 0u; i < dist.size(); ++i)
    {
      float perc = static_cast<float>(i) / cascadeCount;

      float w = weight;

      float uniform = glm::mix(_camera.near, _camera.far, perc);

      float near = glm::max(_camera.near, .000001f); // prevent division by zero. 
      float log = near * glm::pow((_camera.far / near), perc);

      // the logarithmic calculation does not support negative distances so
      // only use the uniform method when negative. 
      if (uniform < .0f)
      {
        w = 0.f;
      }
      dist[i] = glm::mix(uniform, log, w);
    }

    std::vector<glm::vec3> frustumPoints = _camera.getFrustumPoints(dist.data(), dist.size());
    assert(frustumPoints.size() / 4 - 1 == cascadeCount);

    for (size_t i = 0u; i < cascadeCount; ++i)
    {
      glm::vec3 * points = &frustumPoints[i * 4u];

      glm::vec3 min(points[0]);
      glm::vec3 max(points[0]);

      for (size_t i = 1u; i < 8u; ++i)
      {
        min = glm::min(min, points[i]);
        max = glm::max(max, points[i]);
      }

     
      // use sphere instead of aabb so shadow map size is consistent (independent of direction). 
      float radius = glm::length(max - min) * 0.5f;

      glm::vec3 centre = (min + max) * .5f;
      centre = glm::vec3(cam * glm::vec4(centre, 1.f));

      // additional padding to get shadows of objects out of the camera frustum.
      // TODO: search scene for furthest possible caster and adjust padding accordingly.
      float padding = 20.f;
      
      glm::mat4 view = glm::lookAt(centre, centre - _direction, up);
      glm::mat4 proj = glm::ortho(-radius, radius, -radius, radius, -radius - padding, radius);
      glm::mat4 & vp = m_vps[i];
      vp = proj * view;

      // translate vp so it moves in steps of the shadow maps texel size.
      float size = m_shadowBuffer->getWidth() * .5f;

      glm::vec2 origin(vp * glm::vec4(0.f, 0.f, 0.f, 1.f));
      glm::vec2 rounded = glm::round(origin * size) / size;
      glm::vec2 diff = rounded - origin;
      vp = glm::translate(diff.x, diff.y, 0.f) * vp;

      m_distances[i] = dist[1u + i];
    }
  }

  void CascadedShadowRenderer::Render(const CommandBuffer & _commands)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Clear();

    m_depth->Bind();

    m_depth->setUniform<int>("cascadeCount", m_vps.size());

    for (size_t i = 0; i < m_vps.size(); ++i)
    {
      m_depth->setUniform<glm::mat4>("vp[" + std::to_string(i) + ']', m_vps[i]);
    }

    auto & occluders = _commands.getShadowCasters();
    for (auto & command : occluders)
    {
      m_depth->setModel(command.transform);
      command.mesh->Render();
    }

    if (m_blur)
    {
      Texture2DArray & shadow = *m_shadowBuffer->getColourAttachment<Texture2DArray>(0);
      m_blur->Apply(shadow, shadow);
    }
  }

  const CascadedShadowMap CascadedShadowRenderer::getShadowMap() const
  {
    CascadedShadowMap shadow;
    shadow.distances = m_distances;
    shadow.lightSpaces = m_vps;

    shadow.shadowMap = m_shadowBuffer->getColourAttachment<Texture2DArray>(0);
    
    return shadow;
  }

  std::shared_ptr<Texture2DArray> CascadedShadowRenderer::CreateVarienceShadowMap()
  {
    auto texture = std::make_shared<Texture2DArray>(
      TextureFormat::RG32F,
      m_shadowBuffer->getWidth(), m_shadowBuffer->getHeight(), m_shadowBuffer->getDepth(), 
      1
    );

    texture->setBorder(glm::vec4(1.f));
    texture->setWrap(TextureWrap::CLAMP_TO_BORDER);
    texture->setFilter(TextureFilter::LINEAR);

    return texture;
  }

} } // engine::graphics