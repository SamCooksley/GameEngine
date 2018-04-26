#ifndef _ENGINE_GRAPHICS_TEXTURE2D_H_
#define _ENGINE_GRAPHICS_TEXTURE2D_H_

#include "Texture.h"

namespace engine
{
  namespace graphics
  {
    class Texture2D : public Texture
    {
    public:
      static std::shared_ptr<Texture2D> Create(uint _width, uint _height, TextureFormat _format, TextureType _type);
      static std::shared_ptr<Texture2D> Create(uint _width, uint _height, const glm::vec4 & _colour);

      static std::shared_ptr<Texture2D> Load(const std::string & _path, bool _mipmaps = true);

      ~Texture2D();

      void Bind(GLenum _unit) const override;
      void Unbind(GLenum _uint) const override;

      void setWrap(TextureWrap _wrap);
      void setFilter(TextureFilter _filter);

      void setBorder(const glm::vec4 & _colour);

      void GenerateMipMaps();

    protected:
      Texture2D();

    private:
      static std::shared_ptr<Texture2D> Create();

      uint m_width, m_height;

      TextureWrap m_wrap;
      TextureFilter m_filter;
    };
  }
}

#endif //_ENGINE_GRAPHICS_TEXTURE2D_H_
