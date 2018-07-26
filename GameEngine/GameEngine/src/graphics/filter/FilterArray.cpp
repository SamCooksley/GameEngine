#include "stdafx.h"

#include "FilterArray.h"

#include "..\Graphics.h"

namespace engine {
namespace graphics {

  FilterArray::FilterArray(const std::shared_ptr<Shader> & _shader) : Material(_shader),
    m_fb(FrameBuffer::Create(0, 0, 0))
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

    auto restore = Graphics::getContext().activeFrameBuffer.lock();

    m_fb->Bind();
    m_fb->Reset(_dst.getWidth(), _dst.getHeight(), _dst.getDepth());

    _dst.Bind(0);
    m_fb->AttachTemp(_dst, FrameBufferAttachment::COLOUR, 0);

    Bind();
    _src.Bind(m_srcUnit);

    Graphics::RenderQuad();

    restore->Bind();
  }

} } // engine::graphics