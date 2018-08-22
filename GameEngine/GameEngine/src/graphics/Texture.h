#ifndef _ENGINE_GRAPHICS_TEXTURE_H_
#define _ENGINE_GRAPHICS_TEXTURE_H_

#include "TextureTypes.h"

#include "Asset.h"

namespace engine {
namespace graphics {

  class Texture : public NonCopyable, public NamedObject, public Asset
  {
    friend class FrameBuffer;
    friend class DefaultRenderer;

    friend class Texture2D;
    friend class Texture2DArray; 
    friend class TextureCube;

   protected:
    static int CalculateMipMapCount(int _width, int _height);
  
   public:  
    virtual ~Texture();
  
    virtual void Bind() const = 0;
    virtual void Bind(int _unit) const = 0;

    int getWidth() const;
    int getHeight() const;
    int getDepth() const;
    int getMipmapCount() const;

    int getMipMapWidth(int _level) const;
    int getMipMapHeight(int _level) const;

    TextureType getType() const;

    TextureFormat getFormat() const;
    TextureFilter getFilter() const;
    TextureWrap getWrap() const;

   protected:
    Texture(TextureType _type);

   protected:
    GLuint m_id;

    TextureType m_type;

    TextureFormat m_format;
    int m_width, m_height, 
      m_depth, 
      m_mipmaps;

    TextureFilter m_filter;
    TextureWrap m_wrap;
  };
 
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_TEXTURE_H_