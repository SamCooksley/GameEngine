#ifndef _ENGINE_GRAPHICS_SHADOWCUBE_H_
#define _ENGINE_GRAPHICS_SHADOWCUBE_H_

#include "Texture.h"

namespace engine {
namespace graphics {

  class ShadowCube : public Texture
  {
   public:
    static const TextureType type = TextureType::SHADOW_CUBE;

   public:
    ShadowCube(TextureFormat _format, int _width, int _height);
    ~ShadowCube();

    void Bind() const override;
    void Bind(int _unit) const override;
  };

} }

#endif // _ENGINE_GRAPHICS_SHADOWCUBE_H_