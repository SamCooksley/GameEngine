#ifndef _ENGINE_GRAPHICS_SHADOW2DARRAY_H_
#define _ENGINE_GRAPHICS_SHADOW2DARRAY_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Shadow2DArray : public Texture
  {
   public:
    static const TextureType type = TextureType::SHADOW_2D_ARRAY;

   public:
    Shadow2DArray(TextureFormat _format, uint _width, uint _height, uint _count);
    ~Shadow2DArray();

    void Bind(int _unit) const override;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOW2DARRAY_H_