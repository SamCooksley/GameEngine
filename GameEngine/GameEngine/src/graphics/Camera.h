#ifndef _ENGINE_GRAPHICS_CAMERA_H_
#define _ENGINE_GRAPHICS_CAMERA_H_

#include "core\Types.h"

namespace engine {
namespace graphics {
  
  enum class CameraType
  {
    PERSPECTIVE,
    ORTHOGRAPHIC
  };

  struct Camera
  {
   public:
    struct FrustumPositions
    {
      std::array<glm::vec3, 4> positions;
      float distance;
    };

    Camera();
    Camera(
      CameraType _type,
      float _fov, float _aspect, float _near, float _far,
      const glm::mat4 & _projection, const glm::mat4 & _view, 
      const glm::mat4 & _vp,
      const glm::vec3 & _position
    );
    Camera(
      CameraType _type,
      float _fov, float _aspect, float _near, float _far,
      const glm::mat4 & _projection, const glm::mat4 & _view,
      const glm::vec3 & _position
    );
    Camera(
      CameraType _type,
      float _fov, float _aspect, float _near, float _far,
      const glm::mat4 & _view, const glm::vec3 & _position
    );

    float ClipDepthToDistance(float _depth) const;

    std::vector<glm::vec3> getFrustumPoints() const;
    std::vector<glm::vec3> getFrustumPoints(const float * _distances, size_t _size) const;

  private:
    std::vector<glm::vec3> getFrustumPointsPerspective(const float * _distances, size_t _size) const;
    std::vector<glm::vec3> getFrustumPointsOrthographic(const float * _distances, size_t _size) const;

   public:
     CameraType type;

     union
     {
       float fov;
       float size;
     };
     float aspect;

     float near;
     float far;

    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 vp;
  
    glm::vec3 position;
  };
  
} }

#endif //_ENGINE_GRAPHICS_CAMERA_H_