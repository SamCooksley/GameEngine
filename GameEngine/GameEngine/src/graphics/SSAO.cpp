#include "stdafx.h"

#include "SSAO.h"

#include "Random.h"

#include "TextureNoiseGenerator.h"
#include "Graphics.h"
#include "Input.h"

namespace engine {
namespace graphics {

  SSAO::SSAO(const std::shared_ptr<Shader> & _shader, const std::shared_ptr<Filter> & _blur) :
    m_shader(_shader),
    m_blur(_blur)
  {
    m_shader->Bind();  

    int size = m_shader->getUniform<int>("sampleCount");
    size = glm::max(size, 0);
    GenerateKernel(size);
    GenerateNoise();
  }

  SSAO::~SSAO()
  { }

  void SSAO::GenerateAO(Texture2D & _target, const Camera & _camera, const Texture2D & _position, const Texture2D & _normal)
  {
    Texture2D * target = &_target;

    if (m_blur)
    {
      if (!m_bufferTexture
       || !Texture2D::CompareFormat(_target, *m_bufferTexture))
      {
        m_bufferTexture = Texture2D::CloneFormat(_target);
      }
      target = m_bufferTexture.get();
    }

    if (!m_fbo)
    {
      m_fbo = FrameBuffer::Create(target->getWidth(), target->getHeight());
    }
    else
    {
      m_fbo->Bind();
      m_fbo->Reset(target->getWidth(), target->getHeight());
    }

    m_fbo->AttachTemp(*target, FrameBufferAttachment::COLOUR, 0);
    m_fbo->Clear();

    _position.Bind(0);
    _normal.Bind(1);
    m_noise->Bind(2);

    m_shader->Bind();
    m_shader->setUniform("gPosition", 0);
    m_shader->setUniform("gNormal", 1);
    m_shader->setUniform("noise", 2);

    int sampleCount = m_kernel.size();
    m_shader->setUniform<int>("sampleCount", sampleCount);
    for (size_t i = 0u; i < m_kernel.size(); ++i)
    {
      m_shader->setUniform("samples[" + std::to_string(i) + ']', m_kernel[i]);
    }    

    m_shader->setView(_camera.view);
    m_shader->setProjection(_camera.projection);

    Graphics::RenderQuad();

    if (m_blur)
    {
      m_blur->Apply(*target, _target);
    }
  }

  void SSAO::GenerateKernel(size_t _size)
  {
    m_kernel.clear();
    for (size_t i = 0u; i < _size; ++i)
    {
      glm::vec3 sample(
        Random::Range(-1.f, 1.f),
        Random::Range(-1.f, 1.f),
        Random::Range(.01f, 1.f)
      );

      sample = glm::normalize(sample);
      sample *= Random::Value();

      float scale = float(i) / float(_size);
      scale = glm::mix(.1f, 1.f, scale * scale);
      sample *= scale;
      m_kernel.emplace_back(sample);
    }
  }

  void SSAO::GenerateNoise()
  {
    m_noise = TextureNoiseGenerator().GenerateNoise2D(TextureFormat::RG32F, 4, 4, -1.f, 1.f);
    m_blur->setUniform<int>("size", m_noise->getWidth() / 2);
  }

} } // engine::graphics