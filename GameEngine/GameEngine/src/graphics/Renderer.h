#ifndef _ENGINE_GRAPHICS_RENDERER_H_
#define _ENGINE_GRAPHICS_RENDERER_H_

#include "Graphics_Camera.h"

#include "Mesh.h"
#include "Material.h"

namespace engine
{
  namespace graphics
  {
    struct RenderFlags
    {
    public:
      using Type = uint8;
      static constexpr Type None = 0;
      static constexpr Type Shadow = 1 << 0;
    };

    class Renderer
    {
    public:
      virtual ~Renderer();

      RenderFlags::Type getFlags() const;

      virtual void Add(
        std::weak_ptr<Mesh> _mesh, 
        std::weak_ptr<Material> _material, 
        const glm::mat4 & _transform
      );

      virtual void Render() = 0;

    protected:
      Renderer(RenderFlags::Type _flags);

    private:
      RenderFlags::Type m_flags;
    };
  }
}

#endif //_ENGINE_GRAPHICS_RENDERER_H_