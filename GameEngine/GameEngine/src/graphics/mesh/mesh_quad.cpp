#include "stdafx.h"

#include "mesh_quad.h"

namespace engine
{
  namespace graphics
  {
    namespace mesh
    {
      Quad::Quad()
      {
        const std::array<glm::vec3, 4> positions = {
          glm::vec3(-1.f, -1.f, 0.f),
          glm::vec3( 1.f, -1.f, 0.f),
          glm::vec3( 1.f,  1.f, 0.f),
          glm::vec3(-1.f,  1.f, 0.f)
        };

        const std::array<glm::vec3, 4> normals = {
          glm::vec3(0.f, 0.f, 1.f),
          glm::vec3(0.f, 0.f, 1.f),
          glm::vec3(0.f, 0.f, 1.f),
          glm::vec3(0.f, 0.f, 1.f)
        };

        const std::array<glm::vec2, 4> uvs = {
          glm::vec2(0.f, 0.f),
          glm::vec2(1.f, 0.f),
          glm::vec2(1.f, 1.f),
          glm::vec2(0.f, 1.f)
        };

        const std::array<uint8, 4> indices = {
          0, 1, 2, 3
        };

        m_mesh = std::move(Mesh::Create());

        m_mesh->setName("Quad");

        m_mesh->setDraw(DrawType::TRIANGLE_FAN);

        m_mesh->AddVertices(&positions[0], positions.size());
        m_mesh->AddNormals(&normals[0], normals.size());
        m_mesh->AddUVs(&uvs[0], uvs.size());
        m_mesh->setIndices(&indices[0], indices.size());

        m_mesh->Apply();
      }
    }
  }
}