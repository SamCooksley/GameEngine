#ifndef _ENGINE_GRAPHICS_TEXTURE2D_H_
#define _ENGINE_GRAPHICS_TEXTURE2D_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Texture2D : public Texture
  {
   public:
    static const TextureType type = TextureType::TEXTURE_2D;
  
    static std::shared_ptr<Texture2D> Load(const String & _path, int _mipmaps = 0);

   public:
    Texture2D(TextureFormat _format, int _width, int _height, int _mipmaps = 1);
    Texture2D(int _width, int _height, const glm::vec4 & _colour);
    ~Texture2D();
  
    void Bind(int _unit) const override;

    void setWrap(TextureWrap _wrap);
    void setFilter(TextureFilter _filter);
  
    void setBorder(const glm::vec4 & _colour);

    void setAnisotropy(float _anisotropy);

    void GenerateMipMaps();
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE2D_H_
