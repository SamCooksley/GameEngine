#ifndef _ENGINE_GRAPHICS_TEXTURE_H_
#define _ENGINE_GRAPHICS_TEXTURE_H_

#include "TextureTypes.h"

#include "Asset.h"

namespace engine {
namespace graphics {

  class Texture : public NonCopyable, public NamedObject, public Asset
  {
    friend class FrameBuffer;
  
   public:  
    virtual ~Texture();
  
    virtual void Bind(GLenum _unit) const = 0;
    virtual void Unbind(GLenum _unit) const = 0;
  
    virtual void Resize(uint _width, uint _height) = 0;

   protected:
    Texture(TextureType _type);
  
   public:
    const TextureType type;

   protected:
    GLuint m_id;
  };
 
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE_H_