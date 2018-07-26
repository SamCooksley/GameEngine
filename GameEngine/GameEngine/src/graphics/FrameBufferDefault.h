#ifndef _ENGINE_GRAPHICS_FRAMEBUFFERDEFAULT_H_
#define _ENGINE_GRAPHICS_FRAMEBUFFERDEFAULT_H_

#include "FrameBufferBase.h"

namespace engine {
namespace graphics {

  class FrameBufferDefault : public FrameBufferBase
  {
  public:
    static std::shared_ptr<FrameBufferDefault> Create(int _width, int _height);

  public:
    ~FrameBufferDefault();

    void Bind(FrameBufferBind _bind = FrameBufferBind::WRITE) override;

    void Clear();

    void Resize(int _width, int _height);

  protected:
    FrameBufferDefault();
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_FRAMEBUFFERDEFAULT_H_