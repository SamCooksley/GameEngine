#include "stdafx.h"

#include "FilterCube.h"

#include "..\Graphics.h"

namespace engine {
namespace graphics {

  const CameraCube FilterCube::s_camera(glm::vec3(0.f), 0.1f, 2.f);

  FilterCube::FilterCube(const std::shared_ptr<Shader> & _shader) :
    Material(_shader),
    m_srcUnit(0)
  {
    setTexture<TextureCube>("src", nullptr);
    getTextureUnit("src", &m_srcUnit);
  }

  FilterCube::~FilterCube()
  { }

  void FilterCube::Apply(TextureCube & _src, TextureCube & _dst)
  {
    if (&_src == &_dst)
    {
      throw std::invalid_argument("same texture as source and destination");
    }

    auto & fb = Graphics::getContext().captureFBO;
    fb->Bind();
    fb->Reset(_dst.getWidth(), _dst.getHeight(), 1);

    Bind();

    for (size_t i = 0; i < s_camera.size(); ++i)
    {
      getShader()->setUniform<glm::mat4>("vp", s_camera.getCamera(i).vp);

      _dst.Bind();
      fb->AttachTemp(_dst, i, FrameBufferAttachment::COLOUR, 0);

      _src.Bind(m_srcUnit);
      Graphics::RenderCube();
    }
  }

} } // engine::graphics