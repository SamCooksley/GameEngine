#include "stdafx.h"

#include "mesh_skybox.h"

namespace engine
{
  namespace graphics
  {
    namespace mesh
    {
      Skybox::Skybox()
      {
        std::array<glm::vec3, 24> positions = {
          //back
          glm::vec3(-1.f,  1.f, -1.f),
          glm::vec3( 1.f, -1.f, -1.f),
          glm::vec3(-1.f, -1.f, -1.f),
          glm::vec3( 1.f,  1.f, -1.f),

          //front
          glm::vec3( 1.f,  1.f, 1.f),
          glm::vec3(-1.f, -1.f, 1.f),
          glm::vec3( 1.f, -1.f, 1.f),
          glm::vec3(-1.f,  1.f, 1.f),

          //right
          glm::vec3(1.f,  1.f, -1.f),
          glm::vec3(1.f, -1.f,  1.f),
          glm::vec3(1.f, -1.f, -1.f),
          glm::vec3(1.f,  1.f,  1.f),

          //left
          glm::vec3(-1.f,  1.f,  1.f),
          glm::vec3(-1.f, -1.f, -1.f),
          glm::vec3(-1.f, -1.f,  1.f),
          glm::vec3(-1.f,  1.f, -1.f),

          //top
          glm::vec3(-1.f, 1.f,  1.f),
          glm::vec3( 1.f, 1.f, -1.f),
          glm::vec3(-1.f, 1.f, -1.f),
          glm::vec3( 1.f, 1.f,  1.f),

          //bottom
          glm::vec3(-1.f, -1.f, -1.f),
          glm::vec3( 1.f, -1.f,  1.f),
          glm::vec3(-1.f, -1.f,  1.f),
          glm::vec3( 1.f, -1.f, -1.f)
        };

        std::array<uint8, 36> indices = {
          //back
          0, 2, 1,
          0, 1, 3,

          //front
          4, 6, 5,
          4, 5, 7,

          //right
          8, 10,  9,
          8,  9, 11,

          //left
          12, 14, 13,
          12, 13, 15,

          //top
          16, 18, 17,
          16, 17, 19,

          //bottom
          20, 22, 21,
          20, 21, 23
        };

        m_mesh = std::move(Mesh::Create());

        m_mesh->setName("Skybox");

        m_mesh->setDraw(DrawType::TRIANGLES);

        m_mesh->AddVertices(&positions[0], positions.size());
        m_mesh->setIndices(&indices[0], indices.size());

        m_mesh->Apply();
      }
    }
  }
}