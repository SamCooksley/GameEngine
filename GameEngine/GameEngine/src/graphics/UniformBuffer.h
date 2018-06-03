#ifndef _ENGINE_GRAPHICS_UNIFORMBUFFER_H_
#define _ENGINE_GRAPHICS_UNIFORMBUFFER_H_

#include "core\Types.h"
#include "opengl.h"

namespace engine {
namespace graphics {

  class UniformBuffer : public NonCopyable
  {
   public:
    UniformBuffer(String _name);
    virtual ~UniformBuffer();
  
    void Bind() const;
    void Unbind() const;
  
    void Reserve(uint _size);
  
    void setData(const void * _data, uint _size);
    void setData(const void * _data, uint _size, uint _offset);
  
    void setBind(GLuint _index);
    GLuint getBind() const;
  
    const String & getName() const;
  
   private:
    GLuint m_ubo;
    GLuint m_index;
  
    uint m_size;
  
    String m_name;
  };

} } // engine::graphics

#endif //_ENGINE_GRAPHICS_UNIFORMBUFFER_H_
