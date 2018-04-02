#include "stdafx.h"

#include "Renderer.h"

namespace engine
{
  namespace graphics
  {
    Renderer::Renderer(RenderFlags::Type _flags) :
      m_flags(_flags)
    { }

    Renderer::~Renderer()
    { }

    RenderFlags::Type Renderer::getFlags() const
    {
      return m_flags;
    }

    void Renderer::Add(
      std::weak_ptr<Mesh> _mesh,
      std::weak_ptr<Material> _material,
      const glm::mat4 & _transform
    )
    { }
  }
}