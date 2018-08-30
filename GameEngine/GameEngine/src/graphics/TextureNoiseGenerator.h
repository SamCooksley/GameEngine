#ifndef _ENGINE_GRAPHICS_TEXTURENOISEGENERATOR_H_
#define _ENGINE_GRAPHICS_TEXTURENOISEGENERATOR_H_

#include "Texture2D.h"

namespace engine {
namespace graphics {

  class TextureNoiseGenerator
  {
  public:
    std::unique_ptr<Texture2D> GenerateNoise2D(TextureFormat _format, int _width, int _height, float _min, float _max) const;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_TEXTURENOISEGENERATOR_H_