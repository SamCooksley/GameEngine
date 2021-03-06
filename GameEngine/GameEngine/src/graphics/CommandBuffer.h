#ifndef _ENGINE_GRAPHICS_COMMANDBUFFER_H_
#define _ENGINE_GRAPHICS_COMMANDBUFFER_H_

#include "Mesh.h"
#include "Material.h"

#include "Camera.h"

#include "ShadowCommandBuffer.h"

#include "Lights.h"

namespace engine {
namespace graphics {

  struct Command
  {
   public:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    glm::mat4 transform;
  };
  
  class CommandBuffer
  {
   public:
    CommandBuffer();
    ~CommandBuffer();
  
    void Add(
      const std::shared_ptr<Mesh> & _mesh,
      const std::shared_ptr<Material> & _material,
      const glm::mat4 & _transform,
      bool _castShadows = true
    );
  
    void Clear();
  
    void Sort(const Camera & _camera);
  
    const std::vector<Command> & getDeferredCommands() const;
    const std::vector<Command> & getForwardCommands() const;
    const std::vector<Command> & getTransparentCommands() const;

    const std::vector<Command> & getShadowCasters() const;
  
   private:
    std::vector<Command> m_deferred;
    std::vector<Command> m_forward;
    std::vector<Command> m_transparent;
    std::vector<Command> m_shadowCasters;
  };
  
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_COMMANDBUFFER_H_