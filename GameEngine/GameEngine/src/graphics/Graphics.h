#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include "Context.h"

namespace engine {

  class Graphics
  {
    friend class graphics::Shader;
    friend class graphics::Material;
    friend class graphics::UniformBuffers;

    friend class glfw;

   public:
    Graphics() = delete;

    static const std::shared_ptr<graphics::Material> & getDefaultMaterial();

    template <class T>
    static T * getUniformBuffer()
    {
      return getContext().uniformBuffers.getBuffer<T>();
    }

    static graphics::UniformBuffer * getUniformBuffer(const String & _name);

    static graphics::GLData & GL();

    static graphics::Context & getContext();

    static void RenderQuad();
    static void RenderCube();
  };

} // engine

#endif //_ENGINE_GRAPHICS_H_