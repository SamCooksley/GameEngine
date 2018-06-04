#ifndef _ENGINE_GRAPHICS_TEXTURECUBE_H_
#define _ENGINE_GRAPHICS_TEXTURECUBE_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class TextureCube : public Texture
  {
   public:
    static std::shared_ptr<TextureCube> Load(const std::array<String, 6> & _paths);
  
   public:
    TextureCube(uint _width, uint _height, TextureFormat _format, TextureDataType _type);
    TextureCube(uint _width, uint _height, const glm::vec4 & _colour);
  
    ~TextureCube();
  
    void Bind(GLenum _unit) const override;
    void Unbind(GLenum _unit) const override;
  
    void Resize(uint _width, uint _height) override;

    void setFilter(TextureFilter _filter);
  
   protected:
    TextureCube();
  
   private:
    uint m_width, m_height;

    TextureFormat m_format;
    TextureDataType m_dataType;

    TextureFilter m_filter;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURECUBE_H_