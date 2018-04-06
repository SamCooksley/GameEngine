#ifndef _ENGINE_GRAPHICS_CAMERABUFFER_H_
#define _ENGINE_GRAPHICS_CAMERABUFFER_H_

#include "Graphics_Camera.h"

#include "UniformBuffer.h"

namespace engine
{
  namespace graphics
  {
    class CameraBuffer
    {
    public:
      CameraBuffer() = delete;

      static const std::string name;

      static const uint
        projection_offset,
        view_offset,
        vp_offset,
        position_offset,
        
        size;

      static std::unique_ptr<UniformBuffer> Create();
      static void setCamera(UniformBuffer & _buffer, const Camera & _camera);
    };
  }
}

#endif //_ENGINE_GRAPHICS_CAMERABUFFER_H_