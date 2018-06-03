#ifndef _ENGINE_GRAPHICS_CONTEXT_H_
#define _ENGINE_GRAPHICS_CONTEXT_H_

#include "UniformBuffers.h"
#include "Material.h"
#include "BaseRenderer.h"

namespace engine {
namespace graphics {

  struct Context
  {
  public:
    UniformBuffers uniformBuffers;
    GLData glData;

    std::vector<std::weak_ptr<Shader>> shaders;
    std::shared_ptr<Shader> errorShader;

    std::shared_ptr<Material> defaultMaterial;

    std::shared_ptr<BaseRenderer> defaultRenderer;

    std::shared_ptr<Mesh> screenQuad;

    std::weak_ptr<Shader> activeShader;
  };
} // graphics

void Destroy(std::unique_ptr<graphics::Context> & _context);

} // engine

#endif //_ENGINE_GRAPHICS_CONTEXT_H_