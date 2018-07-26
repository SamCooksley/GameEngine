#ifndef _ENGINE_GRAPHICS_FILTER_H_
#define _ENGINE_GRAPHICS_FILTER_H_

#include "..\Material.h"

#include "..\FrameBuffer.h"

#include "..\Texture2D.h"

namespace engine {
namespace graphics {

  class Filter : public Material
  {
  public:
    Filter(const std::shared_ptr<Shader> & _shader);
    ~Filter();

    virtual void Apply(Texture2D & _src, Texture2D & _dst);

  protected:
    int m_srcUnit;

    std::shared_ptr<FrameBuffer> m_fb;
  };

} } // engine::graphic

#endif //_ENGINE_GRAPHICS_FILTER_H_