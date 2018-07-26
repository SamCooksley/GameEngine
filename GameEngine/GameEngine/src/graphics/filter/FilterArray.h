#ifndef _ENGINE_GRAPHICS_FILTERARRAY_H_
#define _ENGINE_GRAPHICS_FILTERARRAY_H_

#include "..\Material.h"

#include "..\FrameBuffer.h"

#include "..\Texture2DArray.h"

namespace engine {
namespace graphics {

  class FilterArray : public Material
  {
  public:
    FilterArray(const std::shared_ptr<Shader> & _shader);
    ~FilterArray();

    virtual void Apply(Texture2DArray & _src, Texture2DArray & _dst);

  protected:
    int m_srcUnit;

    std::shared_ptr<FrameBuffer> m_fb;
  };

} } // engine::graphics

#endif // _ENGINE_GRAPHICS_FILTERARRAY_H_