#ifndef _ENGINE_GRAPHICS_TEXTURE2D_H_
#define _ENGINE_GRAPHICS_TEXTURE2D_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class Texture2D : public Texture
  {
   public:
    static const TextureType type = TextureType::TEXTURE_2D;
  
    static std::shared_ptr<Texture2D> Create(uint _width, uint _height, TextureFormat _format);    
    static std::shared_ptr<Texture2D> Create(uint _width, uint _height, const glm::vec4 & _colour);
  
    static std::shared_ptr<Texture2D> Load(const String & _path, bool _mipmaps = true);

   public:
    Texture2D(uint _width, uint _height, TextureFormat _format);
    ~Texture2D();
  
    void Bind(uint _unit) const override;
    void Unbind(uint _uint) const override;
  
    void Resize(uint _width, uint _height) override;

    void setWrap(TextureWrap _wrap);
    void setFilter(TextureFilter _filter);
  
    void setBorder(const glm::vec4 & _colour);
  
    void GenerateMipMaps();
  
   private:
    uint m_width, m_height;
  
    TextureFormat m_format;

    TextureWrap m_wrap;
    TextureFilter m_filter;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE2D_H_
