#include "stdafx.h"

#include "Texture.h"

namespace engine {
namespace graphics {

  Texture::Texture(TextureType _type) : 
    type(_type), m_id(0)
  { }
  
  Texture::~Texture()
  { }

} } // engine::graphics