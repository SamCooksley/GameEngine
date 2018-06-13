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
    Shadow2DArray(uint _width, uint _height, uint _count, TextureFormat _format);
    ~Shadow2DArray();

    void Bind(uint _unit) const override;
    void Unbind(uint _uint) const override;

    void Resize(uint _width, uint _height) override;

    uint getCount() const;

   private:
    uint m_width, m_height, m_count;

    TextureFormat m_format;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_SHADOW2DARRAY_H_