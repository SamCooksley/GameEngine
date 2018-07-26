#include "stdafx.h"

#include "CameraCube.h"

namespace engine {
namespace graphics {

  CameraCube::CameraCube()
  { }

  CameraCube::CameraCube(const glm::vec3 & _position, float _near, float _far)
  {
    const std::array<glm::vec3, 6> dir = {
      glm::vec3( 1.f,  0.f,  0.f),
      glm::vec3(-1.f,  0.f,  0.f),
      glm::vec3( 0.f,  1.f,  0.f),
      glm::vec3( 0.f, -1.f,  0.f),
      glm::vec3( 0.f,  0.f,  1.f),
      glm::vec3( 0.f,  0.f, -1.f)
    };

    const std::array<glm::vec3, 6> up = {
      glm::vec3(0.f, -1.f,  0.f),
      glm::vec3(0.f, -1.f,  0.f),
      glm::vec3(0.f,  0.f,  1.f),
      glm::vec3(0.f,  0.f, -1.f),
      glm::vec3(0.f, -1.f,  0.f),
      glm::vec3(0.f, -1.f,  0.f)
    };

    for (int i = 0; i < 6; ++i)
    {
      m_cameras[i] = Camera(
        CameraType::PERSPECTIVE, 
        90.f, 1.f,
        _near, _far,
        glm::lookAt(_position, _position + dir[i], up[i]), 
        _position
      );
    }
  }

  const Camera & CameraCube::getCamera(size_t _i) const
  {
    return m_cameras[_i];
  }
  
  const glm::vec3 & CameraCube::getPosition() const
  {
    return m_cameras[0].position;
  }

  float CameraCube::getNear() const
  {
    return m_cameras[0].near;
  }

  float CameraCube::getFar() const
  {
    return m_cameras[0].far;
  }

} } // engine::graphics