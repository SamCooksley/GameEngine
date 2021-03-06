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
  
  void Renderer::Add(const DirectionalLight & _directional)
  { }

  void Renderer::Add(const PointLight & _point)
  { }

  void Renderer::Add(const SpotLight & _spot)
  { }

  void Renderer::Resize(int _width, int _height)
  { }

} } // engine::graphics