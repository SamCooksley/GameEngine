#include "stdafx.h"

#include "BlurArray.h"

#include "..\Graphics.h"

#include "Input.h"

namespace engine {
namespace graphics {

  BlurArray::BlurArray(const std::shared_ptr<Shader> & _shader) : FilterArray(_shader),
    m_passes(1)
  { }

  BlurArray::~BlurArray()
  { }

  void BlurArray::Apply(Texture2DArray & _src, Texture2DArray & _dst)
  {
    if (_src.getDepth() != _dst.getDepth())
    {
      throw std::invalid_argument("different layers");
    }

    auto restore = Graphics::getContext().activeFrameBuffer.lock();

    Bind();
    getShader()->setUniform<int>("layerCount", _src.getDepth());

    m_fb->Bind();
    m_fb->Reset(_dst.getWidth(), _dst.getHeight(), _dst.getDepth());

    if (!m_temp[0] || 
        m_temp[0]->getFormat() != _dst.getFormat() ||
        m_temp[0]->getWidth()  != _dst.getWidth()  ||
        m_temp[0]->getHeight() != _dst.getHeight() ||
        m_temp[0]->getDepth()  != _dst.getDepth()
      )
    {
      for (size_t i = 0; i < m_temp.size(); ++i)
      {
        m_temp[i] = std::make_unique<Texture2DArray>(_dst.getFormat(), _dst.getWidth(), _dst.getHeight(), _dst.getDepth(), 1);
        m_temp[i]->setFilter(_src.getFilter());
      }
    }

    int passes = m_passes * 2;

    for (int pass = 0; pass < passes; ++pass)
    {
      bool pingpong = pass % 2;

      if (pass < passes - 1)
      {
        m_temp[pingpong]->Bind(0);
        m_fb->AttachTemp(*m_temp[pingpong], FrameBufferAttachment::COLOUR, 0);
      }
      else
      {
        _dst.Bind(0);
        m_fb->AttachTemp(_dst, FrameBufferAttachment::COLOUR, 0);
      }

      if (pass == 0)
      {
        _src.Bind(m_srcUnit);
      }
      else
      {
        m_temp[!pingpong]->Bind(0);
      }

      getShader()->setUniform("scale", (pingpong) ? glm::vec2(1, 0) : glm::vec2(0, 1));

      Graphics::RenderQuad();
    }

    restore->Bind();
  }

} } // engine::graphics