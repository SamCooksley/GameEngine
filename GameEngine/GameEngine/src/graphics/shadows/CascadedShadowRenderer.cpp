#include "stdafx.h"

#include "CascadedShadowRenderer.h"

namespace engine {
namespace graphics {

  CascadedShadowRenderer::CascadedShadowRenderer(
    int _size, int _cascadeCount,
    const std::shared_ptr<Shader> & _depth,
    const std::shared_ptr<FilterArray> & _blur
  ) :
    m_cameras(_cascadeCount), m_distances(_cascadeCount),
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
    //debug::Log(std::to_string(_direction.x) + ", " + std::to_string(_direction.y) + ", " +
    //  std::to_string(_direction.z));

    size_t cascadeCount = m_cameras.size();

    m_shadowBuffer->Bind();
    m_shadowBuffer->Attach(CreateVarienceShadowMap(), FrameBufferAttachment::COLOUR, 0);
    m_shadowBuffer->Clear();

    // convert the direction to a transformation matrix.
    glm::vec3 up(0.f, 1.f, 0.f);
    glm::vec3 xaxis = glm::cross(up, _direction);
    xaxis = glm::normalize(xaxis);

    glm::vec3 yaxis = glm::cross(_direction, xaxis);
    yaxis = glm::normalize(yaxis);

    glm::mat4 cam = glm::inverse(_camera.view);

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
      glm::vec3 * points = &frustumPoints[i * 4];

      // create a world space aabb for the frustum split.
      glm::vec3 world = glm::vec3(cam * glm::vec4(points[0], 1.f));

      glm::vec3 min(world);
      glm::vec3 max(world);

      for (size_t i = 1u; i < 8u; ++i)
      {
        world = glm::vec3(cam * glm::vec4(points[i], 1.f));

        min = glm::min(min, world);
        max = glm::max(max, world);
      }

      glm::vec3 centre = (min + max) * .5f;

      // get the size of the frustum split on the lights axes (right, up, forward).

      glm::vec3 halfSize = max - centre;

      std::array<glm::vec3, 4> axes = {
        glm::vec3(halfSize.x,  halfSize.y,  halfSize.z),
        glm::vec3(halfSize.x,  halfSize.y, -halfSize.z),
        glm::vec3(halfSize.x, -halfSize.y, -halfSize.z),
        glm::vec3(halfSize.x, -halfSize.y,  halfSize.z)
      };

      // y axis
      float height = glm::max(
        glm::abs(glm::dot(axes[0], yaxis)),
        glm::abs(glm::dot(axes[1], yaxis)),
        glm::abs(glm::dot(axes[2], yaxis)),
        glm::abs(glm::dot(axes[3], yaxis))
      );

      // x axis
      float width = glm::max(
        glm::abs(glm::dot(axes[0], xaxis)),
        glm::abs(glm::dot(axes[1], xaxis)),
        glm::abs(glm::dot(axes[2], xaxis)),
        glm::abs(glm::dot(axes[3], xaxis))
      );

      // z axis
      float depth = glm::max(
        glm::abs(glm::dot(axes[0], _direction)),
        glm::abs(glm::dot(axes[1], _direction)),
        glm::abs(glm::dot(axes[2], _direction)),
        glm::abs(glm::dot(axes[3], _direction))
      );

      glm::mat4 view = glm::lookAt(centre, centre - _direction, up);

      // additional padding to get shadows of objects out of the camera frustum.
      // TODO: search scene for furthest possible caster and adjust padding accordingly.
      float padding = 20.f;

      m_cameras[i] = Camera(
        graphics::CameraType::ORTHOGRAPHIC,
        height, width / height, 
        -depth - padding, depth,
        view, centre
      );
      m_distances[i] = dist[1 + i];
    }
  }

  void CascadedShadowRenderer::Render(const CommandBuffer & _commands)
  {
    m_shadowBuffer->Bind();
    m_shadowBuffer->Clear();

    m_depth->Bind();

    m_depth->setUniform<int>("cascadeCount", m_cameras.size());

    for (size_t i = 0; i < m_cameras.size(); ++i)
    {
      m_depth->setUniform<glm::mat4>("vp[" + std::to_string(i) + ']', m_cameras[i].vp);
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

    shadow.lightSpaces.reserve(m_cameras.size());
    for (const Camera & cam : m_cameras)
    {
      shadow.lightSpaces.emplace_back(cam.vp);
    }

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