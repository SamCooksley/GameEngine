#ifndef _ENGINE_GRAPHICS_H_
#define _ENGINE_GRAPHICS_H_

#include "Shader.h"
#include "Material.h"

#include "BaseRenderer.h"

#include "VertexArray.h"

#include "Camera.h"

#include "UniformBuffers.h"

#include "GLData.h"

namespace engine
{
  namespace graphics
  {
    struct Context
    {
    public:
      UniformBuffers uniformBuffers;

      std::shared_ptr<Shader> errorShader;

      std::shared_ptr<Material> defaultMaterial;

      std::shared_ptr<BaseRenderer> defaultRenderer;

      std::shared_ptr<Mesh> screenQuad;

      std::vector<std::weak_ptr<::engine::Camera>> cameras;

      std::vector<std::weak_ptr<Shader>> shaders;

      std::weak_ptr<Shader> activeShader;
      std::unique_ptr<GLData> glData;
    };

    class Debug;
  }

  class Graphics
  {
    friend class Application;
    friend class graphics::Shader;
    friend class graphics::Material;
    friend class Camera;
    friend class graphics::UniformBuffers;
    friend class graphics::Debug;

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

    static std::shared_ptr<graphics::BaseRenderer> getDefaultRenderer();

    static void AddCamera(std::shared_ptr<Camera> _camera);
    static void RemoveCamera(std::shared_ptr<Camera> _camera);

    static graphics::Context & getContext();
  };
}

#endif //_ENGINE_GRAPHICS_H_