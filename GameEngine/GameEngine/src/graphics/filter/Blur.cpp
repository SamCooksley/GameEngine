#include "stdafx.h"

#include "Blur.h"

#include "..\Graphics.h"

namespace engine {
namespace graphics {

  Blur::Blur(const std::shared_ptr<Shader> & _shader) : Filter(_shader),
    m_passes(1)
  { }

  Blur::~Blur()
  { }

  void Blur::Apply(Texture2D & _src, Texture2D & _dst)
  {    
    auto & fb = Graphics::getContext().captureFBO;
    fb->Bind();
    fb->Reset(_dst.getWidth(), _dst.getHeight());

    if (!m_temp[0] ||
         m_temp[0]->getFormat() != _dst.getFormat() ||
         m_temp[0]->getWidth()  != _dst.getWidth() ||
         m_temp[0]->getHeight() != _dst.getHeight()
      )
    {
      for (size_t i = 0; i < m_temp.size(); ++i)
      {
        m_temp[i] = std::make_unique<Texture2D>(_dst.getFormat(), _dst.getWidth(), _dst.getHeight(), 1);
        m_temp[i]->setFilter(_src.getFilter());
        m_temp[i]->setWrap(_src.getWrap());
      }
    }

    Bind();

    int passes = m_passes * 2;

    for (int pass = 0; pass < passes; ++pass)
    {
      bool pingpong = pass % 2;

      if (pass < passes - 1)
      {
        m_temp[pingpong]->Bind();
        fb->AttachTemp(*m_temp[pingpong], FrameBufferAttachment::COLOUR, 0);
      }
      else
      {
        _dst.Bind();
        fb->AttachTemp(_dst, FrameBufferAttachment::COLOUR, 0);
      }

      if (pass == 0)
      {
        _src.Bind(m_srcUnit);
      }
      else
      {
        m_temp[!pingpong]->Bind(m_srcUnit);
      }

      getShader()->setUniform("scale", (pingpong) ? glm::vec2(1, 0) : glm::vec2(0, 1));

      Graphics::RenderQuad();
    }
  }

} } // engine::graphics