#include "stdafx.h"

#include "Mesh.h"

#include "debug\Debug.h"

namespace engine
{
  namespace graphics
  {
    std::shared_ptr<Mesh> Mesh::Load(const std::string & _path)
    {
      return nullptr;
    }

    Mesh::Mesh()
    { }

    Mesh::~Mesh()
    { }

    void Mesh::Render() const
    {
      //get currently active shader.
      //Render(Graphics::getActiveShader());
    }

    void Mesh::Render(Shader & _shader) const
    {
      if (!m_vbo || !m_indices)
      {
        debug::LogError("Mesh does not exist");
        return;
      }

      m_vbo->Bind();

      size_t attrCount = m_attributes.size();
      std::vector<ShaderAttribute> attrs(attrCount);

      for (size_t i = 0; i < attrCount; ++i)
      {
        const VertexAttribute & vertAttr = m_attributes[i];
        ShaderAttribute & attr = attrs[i];
        attr.location = -1;

        //if (!_shader.getAttribute(vertAttr.name, &attr))
        {
          if (!_shader.AddAttribute(vertAttr.name, vertAttr.type, vertAttr.count, false, &attr))
          {
            continue;
          }
        }

        if (attr.location < 0) { continue; }

        GLCALL(glEnableVertexAttribArray(attr.location));
        GLCALL(glVertexAttribPointer(attr.location, attr.count, attr.type, attr.normalized, 0, (const void *)vertAttr.offset));
      }


      m_indices->Bind();
      m_indices->Draw(m_draw);
      m_indices->Unbind();

      for (auto & attr : attrs)
      {
        if (attr.location >= 0)
        {
          GLCALL(glDisableVertexAttribArray(attr.location));
        }
      }

      m_vbo->Unbind();
    }

    void Mesh::setIndices(const uint16 * _indices, uint _count)
    {
      m_indexData.resize(_count);
      memcpy(&m_indexData[0], _indices, sizeof(uint16) * _count);
    }

    void Mesh::AddAttribute(const std::string & _name, const void * _data, uint _size)
    {
      uint start = m_vboData.size();
      m_vboData.resize(start + _size);

      memcpy(&m_vboData[start], _data, _size);


    }
  }
}