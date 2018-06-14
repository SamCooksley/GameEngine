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
    Texture2DArray(uint _width, uint _height, uint _layers, uint _mipmaps, TextureFormat _format);
    ~Texture2DArray();

    void Bind(uint _unit) const override;
    void Unbind(uint _uint) const override;

    void Resize(uint _width, uint _height) override;

    void setWrap(TextureWrap _wrap);
    void setFilter(TextureFilter _filter);

    void setBorder(const glm::vec4 & _colour);

    void GenerateMipMaps();

  private:
    uint m_width, m_height, m_layers;

    TextureFormat m_format;

    TextureWrap m_wrap;
    TextureFilter m_filter;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE2DARRAY_H_