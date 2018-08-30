#ifndef _ENGINE_GRAPHICS_SSAO_H_
#define _ENGINE_GRAPHICS_SSAO_H_

#include "Camera.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "filter\Filter.h"
#include "filter\Blur.h"
namespace engine {
namespace graphics {

  class SSAO
  {
  public:
    SSAO(
      const std::shared_ptr<Shader> & _shader = Shader::Load("resources/shaders/lighting/ssao.glsl"), 
      const std::shared_ptr<Filter> & _blur = std::make_shared<Filter>(Shader::Load("resources/shaders/filters/blur_simple.glsl"))
    );
    ~SSAO(); 

    void GenerateAO(Texture2D & _target, const Camera & _camera, const Texture2D & _position, const Texture2D & _normal);

  private:
    void GenerateKernel(size_t _size);
    void GenerateNoise();

  private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<FrameBuffer> m_fbo;

    std::vector<glm::vec3> m_kernel;
    std::unique_ptr<Texture2D> m_noise;

    std::shared_ptr<Filter> m_blur;

    std::unique_ptr<Texture2D> m_bufferTexture;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_SSAO_H_