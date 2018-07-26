#include "stdafx.h"

#include "FrameBufferDefault.h"

#include "Graphics.h"

namespace engine {
namespace graphics {

  std::shared_ptr<FrameBufferDefault> FrameBufferDefault::Create(int _width, int _height)
  {
    class enable_fbd : public FrameBufferDefault { };

    auto fbd = std::make_shared<enable_fbd>();
    fbd->Bind();
    fbd->Resize(_width, _height);
    return fbd;
  }

  FrameBufferDefault::FrameBufferDefault()
  {
    m_depth = 1;
  }

  FrameBufferDefault::~FrameBufferDefault()
  { }

  void FrameBufferDefault::Bind(FrameBufferBind _bind)
  {
    glBindFramebuffer(FrameBufferBindToOpenGL(_bind), 0);

    if (_bind == FrameBufferBind::WRITE)
    {
      glViewport(0, 0, m_width, m_height);
      Graphics::getContext().activeFrameBuffer = shared_from_this();
    }
  }

  void FrameBufferDefault::Clear()
  {
    glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
    glClear(BufferBit::COLOUR | BufferBit::DEPTH);
  }

  void FrameBufferDefault::Resize(int _width, int _height)
  {
    m_width = _width;
    m_height = _height;
  }

} } // engine::graphics