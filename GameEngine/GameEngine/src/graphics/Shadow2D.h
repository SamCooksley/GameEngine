#ifndef _ENGINE_GRAPHICS_SHADOW2D_H_
#define _ENGINE_GRAPHICS_SHADOW2D_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Shadow2D : public Texture
  {
   public:
    static const TextureType type = TextureType::SHADOW_2D;

    static std::shared_ptr<Shadow2D> Create(uint _width, uint _height, TextureFormat _format);

   public:
    ~Shadow2D();

    void Bind(GLenum _unit) const override;
    void Unbind(GLenum _unit) const override;

    void Resize(uint _width, uint _height) override;

   protected:
     Shadow2D();

  private:
    uint m_width, m_height;

    TextureFormat m_format;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOW2D_H_