#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include "Shader.h"
#include "Material.h"

#include "BaseRenderer.h"

#include "VertexArray.h"

namespace engine
{
  namespace graphics
  {
    struct Context
    {
    public:
      std::shared_ptr<Shader> errorShader;

      std::shared_ptr<Material> defaultMaterial;

      std::unique_ptr<BaseRenderer> renderer;

      std::unique_ptr<VertexArray> vao;
    };
  }

  class Graphics
  {
    friend class graphics::Shader;
    friend class graphics::Material;

  public:
    Graphics() = delete;

    static std::shared_ptr<graphics::Material> getDefaultMaterial();

  private:
    static std::shared_ptr<graphics::Shader> getErrorShader();
  };
}

#endif //_ENGINE_GRAPHICS_H_