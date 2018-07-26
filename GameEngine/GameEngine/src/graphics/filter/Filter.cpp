#include "stdafx.h"

#include "Filter.h"

#include "..\Graphics.h"

#include "utilities\File.h"

namespace engine {
namespace graphics {

  Filter::Filter(const std::shared_ptr<Shader> & _shader) : Material(_shader),
    m_srcUnit(0),
    m_fb(FrameBuffer::Create(0, 0, 0))
  {
    setTexture<Texture2D>("src", nullptr);
    getTextureUnit("src", &m_srcUnit);
  }

  Filter::~Filter()
  { }

  void Filter::Apply(Texture2D & _src, Texture2D & _dst)
  {
    if (&_src == &_dst)
    {
      throw std::invalid_argument("same texture as source and destination");
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

} }