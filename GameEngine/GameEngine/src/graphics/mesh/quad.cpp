#include "stdafx.h"

#include "quad.h"

namespace engine {
namespace graphics {
namespace mesh {

  Quad::Quad()
  {
    MeshData data;
    data.positions = {
      glm::vec3(-1.f, -1.f, 0.f),
      glm::vec3( 1.f, -1.f, 0.f),
      glm::vec3( 1.f,  1.f, 0.f),
      glm::vec3(-1.f,  1.f, 0.f)
    };
  
    data.uvs = {
      glm::vec2(0.f, 0.f),
      glm::vec2(1.f, 0.f),
      glm::vec2(1.f, 1.f),
      glm::vec2(0.f, 1.f)
    };
  
    data.normals = {
      glm::vec3(0.f, 0.f, 1.f),
      glm::vec3(0.f, 0.f, 1.f),
      glm::vec3(0.f, 0.f, 1.f),
      glm::vec3(0.f, 0.f, 1.f)
    };
  
    data.indices.setIndices({
      0, 1, 2, 3
    });
  
    m_mesh = std::make_shared<Mesh>(data, DrawType::TRIANGLE_FAN);
    m_mesh->setName("Quad");
  }

} } } // engine::graphics::mesh