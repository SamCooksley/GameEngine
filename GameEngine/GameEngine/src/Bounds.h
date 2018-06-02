#ifndef _ENGINE_BOUNDS_H_
#define _ENGINE_BOUNDS_H_

#include "core/Types.h"

namespace engine
{
  class Bounds
  {
  public:
    glm::vec3 min;
    glm::vec3 max;

    Bounds();
    Bounds(const glm::vec3 & _min, const glm::vec3 & _max);
    ~Bounds();

    glm::vec3 Size() const;

    bool Contains(const glm::vec3 & _point) const;
    float Distance(const glm::vec3 & _point) const;
    float Distance2(const glm::vec3 & _point) const;

    bool Intersects(const Bounds & _other) const;
    bool Contains(const Bounds & _other) const;

    Bounds Multiply(const glm::mat4 & _transform) const;
  };
}

#endif //_ENGINE_BOUNDS_H_