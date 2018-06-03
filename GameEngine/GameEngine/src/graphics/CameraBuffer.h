#ifndef _ENGINE_GRAPHICS_CAMERABUFFER_H_
#define _ENGINE_GRAPHICS_CAMERABUFFER_H_

#include "Camera.h"

#include "UniformBuffer.h"

namespace engine {
namespace graphics {

  class CameraBuffer : public UniformBuffer
  {
   public:
    static const String name;
  
   private:
    static const uint
      projection_offset,
      view_offset,
      vp_offset,
      position_offset,
  
      size;
  
   public:
    CameraBuffer();
    ~CameraBuffer();
  
    void setCamera(const Camera & _camera);
  };
  
} } // engine::graphics

#endif //_ENGINE_GRAPHICS_CAMERABUFFER_H_