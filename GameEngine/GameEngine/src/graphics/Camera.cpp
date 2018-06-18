#include "stdafx.h"

#include "Camera.h"

namespace engine {
namespace graphics {

  Camera::Camera() :
    type(CameraType::ORTHOGRAPHIC),
    fov(0.f), aspect(1.f), near(0.f), far(0.f),
    projection(1.f), view(1.f),
    vp(1.f),
    position(0.f)
  { }

  Camera::Camera(
    CameraType _type,
    float _fov, float _aspect, float _near, float _far,
    const glm::mat4 & _projection, const glm::mat4 & _view,
    const glm::mat4 & _vp,
    const glm::vec3 & _position
  ) :
    type(_type),
    fov(_fov), aspect(_aspect), near(_near), far(_far),
    projection(_projection), view(_view), 
    vp(_vp),
    position(_position)
  { }

  Camera::Camera(
    CameraType _type,
    float _fov, float _aspect, float _near, float _far,
    const glm::mat4 & _projection, const glm::mat4 & _view,
    const glm::vec3 & _position
  ) :
    type(_type),
    fov(_fov), aspect(_aspect), near(_near), far(_far),
    projection(_projection), view(_view),
    vp(_projection * _view),
    position(_position)
  { }

  Camera::Camera(
    CameraType _type,
    float _fov, float _aspect, float _near, float _far,
    const glm::mat4 & _view, const glm::vec3 & _position
  ) :
    type(_type),
    fov(_fov), aspect(_aspect), near(_near), far(_far),
    projection(
      _type == CameraType::PERSPECTIVE ? 
        glm::perspective(_fov, _aspect, _near, _far) :
        glm::ortho(-_fov * _aspect, _fov * _aspect, -_fov, _fov, _near, _far) 
    ),
    view(_view),
    vp(projection * view),
    position(_position)
  { }

  float Camera::ClipDepthToDistance(float _depth) const
  {
    return near + (far - near) * _depth;
  }

  std::vector<glm::vec3> Camera::getFrustumPoints() const
  {
    float clipping[2] = { 0.f, 1.f };
    return getFrustumPoints(clipping, 2);
  }

  std::vector<glm::vec3> Camera::getFrustumPoints(const float * _distances, size_t _size) const
  {
    if (type == CameraType::PERSPECTIVE)
    {
      return getFrustumPointsPerspective(_distances, _size);
    }

    return getFrustumPointsOrthographic(_distances, _size);
  }

  std::vector<glm::vec3> Camera::getFrustumPointsPerspective(const float * _distances, size_t _size) const
  {
    float fovy = glm::radians(fov * .5f);
    float fovx = glm::radians(fov * aspect * .5f);

    float tay = glm::tan(fovy);
    float tax = glm::tan(fovx);

    const std::array<glm::vec2, 4u> dir = {
      glm::vec2(-1.f, -1.f),
      glm::vec2( 1.f, -1.f),
      glm::vec2( 1.f,  1.f),
      glm::vec2(-1.f,  1.f)
    };

    std::vector<glm::vec3> results(_size * dir.size());

    for (size_t i = 0u; i < _size; ++i)
    {
      for (size_t j = 0u; j < dir.size(); ++j)
      {
        results[i * 4u + j] = glm::vec3(
          tax * _distances[i] * dir[j].x,
          tay * _distances[i] * dir[j].y,
          -_distances[i] // invert distance as camera forward is negative.
        );
      }
    }

    return results;
  }

  std::vector<glm::vec3> Camera::getFrustumPointsOrthographic(const float * _distances, size_t _size) const
  {
    float y = size;
    float x = size * aspect;

    const std::array<glm::vec2, 4u> dir = {
      glm::vec2(-1.f, -1.f),
      glm::vec2( 1.f, -1.f),
      glm::vec2( 1.f,  1.f),
      glm::vec2(-1.f,  1.f)
    };

    std::vector<glm::vec3> results(_size * 4u);

    for (size_t i = 0u; i < _size; ++i)
    {
      for (size_t j = 0u; j < dir.size(); ++j)
      {
        results[i * 4u + j] = glm::vec3(
          x * dir[j].x,
          y * dir[j].y,
          -_distances[i] // invert distance as camera forward is negative.
        );
      }
    }

    return results;
  }

} } // engine::graphics