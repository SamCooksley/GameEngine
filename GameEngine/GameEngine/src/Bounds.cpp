#include "stdafx.h"

#include "Bounds.h"

namespace engine {

  Bounds::Bounds()
  { }

  Bounds::Bounds(const glm::vec3 & _min, const glm::vec3 & _max) :
    min(_min), max(_max)
  { }

  Bounds::~Bounds()
  { }

  glm::vec3 Bounds::Size() const
  {
    return max - min;
  }

  bool Bounds::Contains(const glm::vec3 & _point) const
  {
    return
      _point.x >= min.x && _point.x <= max.x &&
      _point.y >= min.y && _point.y <= max.y &&
      _point.z >= min.z && _point.z <= max.z;
  }

  float Bounds::Distance(const glm::vec3 & _point) const
  {
    glm::vec3 dist(
      glm::max(0.f, min.x - _point.x, _point.x - max.x),
      glm::max(0.f, min.y - _point.y, _point.y - max.y),
      glm::max(0.f, min.z - _point.z, _point.z - max.z)
    );
    return glm::length(dist);
  }

  float Bounds::Distance2(const glm::vec3 & _point) const
  {
    glm::vec3 dist(
      glm::max(0.f, min.x - _point.x, _point.x - max.x),
      glm::max(0.f, min.y - _point.y, _point.y - max.y),
      glm::max(0.f, min.z - _point.z, _point.z - max.z)
    );
    return glm::length2(dist);
  }

  bool Bounds::Intersects(const Bounds & _other) const
  {
    return 
      this->max.x >= _other.min.x && this->min.x <= _other.max.x &&
      this->max.y >= _other.min.y && this->min.y <= _other.max.y &&
      this->max.z >= _other.min.z && this->min.z <= _other.max.z;
  }

  bool Bounds::Contains(const Bounds & _other) const
  {
    return Contains(_other.min) && Contains(_other.max);
  }

  Bounds Bounds::Multiply(const glm::mat4 & _transform) const
  {
    glm::mat4x3 trs(_transform);

    glm::vec3 xmin = trs[0] * min.x;
    glm::vec3 xmax = trs[0] * max.x;

    glm::vec3 ymin = trs[1] * min.y;
    glm::vec3 ymax = trs[1] * max.y;

    glm::vec3 zmin = trs[2] * min.z;
    glm::vec3 zmax = trs[2] * max.z;

    return Bounds(
      glm::min(xmin, xmax) + glm::min(ymin, ymax) + glm::min(zmin, zmax) + trs[3],
      glm::max(xmin, xmax) + glm::max(ymin, ymax) + glm::max(zmin, zmax) + trs[3]
    );
  }

} // engine