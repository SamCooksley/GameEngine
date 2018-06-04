#ifndef _ENGINE_GRAPHICS_RENDERER_H_
#define _ENGINE_GRAPHICS_RENDERER_H_

#include "Camera.h"
#include "Light.h"

#include "Mesh.h"
#include "Material.h"

namespace engine {
namespace graphics {

  struct RenderFlags
  {
   public:
    using Type = uint8;
    static constexpr Type None = 0;
    static constexpr Type Shadow = 1 << 0;
    static constexpr Type Debug = 1 << 1;
  };
  
  class Renderer
  {
   public:
    virtual ~Renderer();
  
    RenderFlags::Type getFlags() const;
  
    virtual void Add(
      const std::shared_ptr<Mesh> & _mesh, 
      const std::shared_ptr<Material> & _material,
      const glm::mat4 & _transform
    );
  
    virtual void Add(const Light & _light);
  
    virtual void Render() = 0;

    virtual void Resize(uint _width, uint _height);
  
   protected:
    Renderer(RenderFlags::Type _flags);
  
   private:
    RenderFlags::Type m_flags;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_RENDERER_H_