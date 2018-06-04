#include "stdafx.h"

#include "Renderer.h"

namespace engine {
namespace graphics {

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
    const std::shared_ptr<Mesh> & _mesh,
    const std::shared_ptr<Material> & _material,
    const glm::mat4 & _transform
  )
  { }
  
  void Renderer::Add(const Light & _light)
  { }

  void Renderer::Resize(uint _width, uint _height)
  { }

} } // engine::graphics