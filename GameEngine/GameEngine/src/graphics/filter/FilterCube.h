#ifndef _ENGINE_GRAPHICS_FILTERCUBE_H_
#define _ENGINE_GRAPHICS_FILTERCUBE_H_

#include "..\Material.h"

#include "..\FrameBuffer.h"

#include "..\CameraCube.h"

namespace engine {
namespace graphics {

  class FilterCube : public Material
  {
  private:
    static const CameraCube s_camera;

  public:
    FilterCube(const std::shared_ptr<Shader> & _shader);
    ~FilterCube();

    void Apply(TextureCube & _src, TextureCube & _dst);

  private:
    int m_srcUnit;

    std::shared_ptr<Material> m_material;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_FILTERCUBE_H_