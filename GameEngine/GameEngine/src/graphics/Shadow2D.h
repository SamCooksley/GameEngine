#ifndef _ENGINE_GRAPHICS_SHADOW2D_H_
#define _ENGINE_GRAPHICS_SHADOW2D_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Shadow2D : public Texture
  {
   public:
    static const TextureType type = TextureType::SHADOW_2D;

   public:
    Shadow2D(TextureFormat _format, int _width, int _height);
    ~Shadow2D();

    void Bind(int _unit) const override;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOW2D_H_