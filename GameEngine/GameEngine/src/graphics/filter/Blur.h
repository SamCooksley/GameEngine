#ifndef _ENGINE_GRAPHICS_BLUR_H_
#define _ENGINE_GRAPHICS_BLUR_H_

#include "Filter.h"

namespace engine {
namespace graphics {

  class Blur : public Filter
  {
  public:
    Blur(const std::shared_ptr<Shader> & _shader = Shader::Load("resources/shaders/filters/blur.glsl"));
    ~Blur();

    void Apply(Texture2D & _src, Texture2D & _dst) override;

  private:
    int m_passes;

    std::array<std::unique_ptr<Texture2D>, 2> m_temp;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_BLUR_H_