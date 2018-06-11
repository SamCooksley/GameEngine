#ifndef _ENGINE_GRAPHICS_SHADOWCOMMANDBUFFER_H_
#define _ENGINE_GRAPHICS_SHADOWCOMMANDBUFFER_H_

#include "Mesh.h"

namespace engine {
namespace graphics {

  struct ShadowCommand
  {
  public:
    std::shared_ptr<Mesh> mesh;
    glm::mat4 transform;
  };

  class ShadowCommandBuffer
  {
   public:
    ShadowCommandBuffer() = default;
    ~ShadowCommandBuffer() = default;

    void Add(
      const std::shared_ptr<Mesh> & _mesh,
      const glm::mat4 & _transform
    );

    void Clear();

    const std::vector<ShadowCommand> & getCommands() const;

   private:
    std::vector<ShadowCommand> m_commands;
  };

} } // engine::graphics



#endif //_ENGINE_GRAPHICS_SHADOWCOMMANDBUFFER_H_