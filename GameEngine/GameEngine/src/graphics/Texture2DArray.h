#ifndef _ENGINE_GRAPHICS_TEXTURE2DARRAY_H_
#define _ENGINE_GRAPHICS_TEXTURE2DARRAY_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Texture2DArray : public Texture
  {
   public:
    static const TextureType type = TextureType::TEXTURE_2D_ARRAY;

   public:
    Texture2DArray(TextureFormat _format, int _width, int _height, int _layers, int _mipmaps);
    ~Texture2DArray();

    void Bind() const override;
    void Bind(int _unit) const override;
    
    void setWrap(TextureWrap _wrap);
    void setFilter(TextureFilter _filter);

    void setBorder(const glm::vec4 & _colour);

    void setAnisotropy(float _anisotropy);

    void GenerateMipMaps();
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE2DARRAY_H_