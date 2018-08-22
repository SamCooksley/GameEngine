#ifndef _ENGINE_GRAPHICS_BLURARRAY_H_
#define _ENGINE_GRAPHICS_BLURARRAY_H_

#include "FilterArray.h"

namespace engine {
namespace graphics {

  class BlurArray : public FilterArray
  {
  public:
    BlurArray(
      const std::shared_ptr<Shader> & _shader = Shader::Load("resources/shaders/filters/blur_array.glsl")
    );
    ~BlurArray();

    void Apply(Texture2DArray & _src, Texture2DArray & _dst) override;

  private:
    int m_passes;

    std::array<std::unique_ptr<Texture2DArray>, 2> m_temp;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_BLURARRAY_H_