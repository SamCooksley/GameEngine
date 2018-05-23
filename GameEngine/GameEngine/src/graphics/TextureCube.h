#ifndef _ENGINE_GRAPHICS_TEXTURECUBE_H_
#define _ENGINE_GRAPHICS_TEXTURECUBE_H_

#include "Texture.h"

namespace engine
{
  namespace graphics
  {
    class TextureCube : public Texture
    {
    public:
      static std::shared_ptr<TextureCube> Load(const std::array<std::string, 6> & _paths);

      TextureCube(uint _width, uint _height, TextureFormat _format, TextureDataType _type);
      TextureCube(uint _width, uint _height, const glm::vec4 & _colour);

      ~TextureCube();

      void Bind(GLenum _unit) const override;
      void Unbind(GLenum _unit) const override;

      void setFilter(TextureFilter _filter);

    protected:
      TextureCube();

    private:
      uint m_width, m_height;
      TextureFilter m_filter;
    };
  }
}

#endif //_ENGINE_GRAPHICS_TEXTURECUBE_H_