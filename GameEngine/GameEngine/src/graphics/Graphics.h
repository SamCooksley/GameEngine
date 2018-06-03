#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include "graphics\Context.h"

namespace engine
{
  class Graphics
  {
    friend class graphics::Shader;
    friend class graphics::Material;
    friend class graphics::UniformBuffers;
    friend class Camera;

  public:
    Graphics() = delete;

    static const std::shared_ptr<graphics::Material> & getDefaultMaterial();

    template <class T>
    static T * getUniformBuffer()
    {
      return getContext().uniformBuffers.getBuffer<T>();
    }

    static graphics::UniformBuffer * getUniformBuffer(const std::string & _name);

    static graphics::GLData & GL();

  private:
    static std::shared_ptr<graphics::Shader> getErrorShader();

    static graphics::Context & getContext();
  };
}

#endif //_ENGINE_GRAPHICS_H_