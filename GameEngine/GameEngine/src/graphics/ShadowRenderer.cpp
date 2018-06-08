#include "stdafx.h"

#include "ShadowRenderer.h"

namespace engine {
namespace graphics {

  void ShadowCommandBuffer::Add(
    const std::shared_ptr<Mesh> & _mesh,
    const glm::mat4 & _transform
  )
  {
    assert(_mesh);

    m_commands.push_back({ _mesh, _transform });
  }

  void ShadowCommandBuffer::Clear()
  {
    m_commands.clear();
  }

  const std::vector<ShadowCommand> & ShadowCommandBuffer::getCommands() const
  {
    return m_commands;
  }

  ShadowRenderer::ShadowRenderer(const std::shared_ptr<Material> & _material) :
    Renderer(RenderFlags::Shadow),
    m_material(_material)
  {
    assert(_material);
  }

  ShadowRenderer::~ShadowRenderer()
  { }

  void ShadowRenderer::Start(const Camera & _camera)
  {
    Reset();

    m_camera = _camera;
  }

  void ShadowRenderer::Add(
    const std::shared_ptr<Mesh> & _mesh,
    const std::shared_ptr<Material> & _material,
    const glm::mat4 & _transform
  )
  {
    m_commands.Add(_mesh, _transform);
  }

  void ShadowRenderer::End()
  { }

  void ShadowRenderer::Render()
  {
    m_material->Bind();
    auto shader = m_material->getShader();
    shader->setProjection(m_camera.projection);
    shader->setView(m_camera.view);
    
    auto & commands = m_commands.getCommands();
    for (auto & command : commands)
    {
      shader->setModel(command.transform);
      command.mesh->Render();
    }
  }

  void ShadowRenderer::Reset()
  {
    m_commands.Clear();
  }

} } // engine::graphics