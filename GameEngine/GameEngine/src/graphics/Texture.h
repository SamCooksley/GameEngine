#ifndef _ENGINE_GRAPHICS_TEXTURE_H_
#define _ENGINE_GRAPHICS_TEXTURE_H_

#include "core\Object.h"

#include "TextureTypes.h"

namespace engine
{
  namespace graphics
  {
    class Texture : public Object, public NonCopyable
    {
      friend class FrameBuffer;

    public:
      const TextureType type;

      virtual ~Texture();

      virtual void Bind(GLenum _unit) const = 0;
      virtual void Unbind(GLenum _unit) const = 0;

    protected:
      Texture(TextureType _type);

      GLuint m_id;
    };
  }
}

#endif //_ENGINE_GRAPHICS_TEXTURE_H_