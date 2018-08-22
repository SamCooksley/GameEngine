#ifndef _ENGINE_GRAPHICS_TEXTURECUBE_H_
#define _ENGINE_GRAPHICS_TEXTURECUBE_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class TextureCube : public Texture
  {
   public:
    static const TextureType type = TextureType::TEXTURE_CUBE;

    static std::shared_ptr<TextureCube> Load(const std::array<String, 6> & _paths, bool _linear = false, int _mipmaps = 0);
  
   public:
    TextureCube(TextureFormat _format, int _width, int _height, int _mipmaps);  
    TextureCube(const TextureCube & _src, int _level, int _levelCount = 1);
    ~TextureCube();
  
    void Bind() const override;
    void Bind(int _unit) const override;

    void setFilter(TextureFilter _filter);

    void setAnisotropy(float _anisotropy);

    void GenerateMipMaps();
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURECUBE_H_