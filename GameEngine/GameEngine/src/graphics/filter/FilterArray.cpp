#include "stdafx.h"

#include "FilterArray.h"

#include "..\Graphics.h"

namespace engine {
namespace graphics {

  FilterArray::FilterArray(const std::shared_ptr<Shader> & _shader) :
    Material(_shader),
    m_srcUnit(0)
  {
    setTexture<Texture2DArray>("src", nullptr);
    getTextureUnit("src", &m_srcUnit);
  }

  FilterArray::~FilterArray()
  { }

  void FilterArray::Apply(Texture2DArray & _src, Texture2DArray & _dst)
  {
    if (_src.getDepth() != _dst.getDepth())
    {
      throw std::invalid_argument("different layers");
    }
    
    auto & fb = Graphics::getContext().captureFBO;
    fb->Bind();
    fb->Reset(_dst.getWidth(), _dst.getHeight(), _dst.getDepth());

    _dst.Bind();
    fb->AttachTemp(_dst, FrameBufferAttachment::COLOUR, 0);

    Bind();
    _src.Bind(m_srcUnit);

    Graphics::RenderQuad();
  }

} } // engine::graphics