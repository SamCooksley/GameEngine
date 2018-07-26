#include "stdafx.h"

#include "FrameBufferBase.h"

namespace engine {
namespace graphics {

  FrameBufferBase::FrameBufferBase() :
    m_width(0), m_height(0), m_depth(0),
    m_clearColour(0.f)
  { }

  FrameBufferBase::~FrameBufferBase()
  { }

  void FrameBufferBase::Clear()
  {

  }

  int FrameBufferBase::getWidth() const
  {
    return m_width;
  }

  int FrameBufferBase::getHeight() const
  {
    return m_height;
  }

  int FrameBufferBase::getDepth() const
  {
    return m_depth;
  }

  void FrameBufferBase::setClearColour(const glm::vec4 & _colour)
  {
    m_clearColour = _colour;
  }

} } // engine::graphics 