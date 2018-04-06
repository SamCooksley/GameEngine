#ifndef _ENGINE_GRAPHICS_UNIFORMBUFFER_H_
#define _ENGINE_GRAPHICS_UNIFORMBUFFER_H_

#include "core\Types.h"

#include "opengl.h"

namespace engine
{
  namespace graphics
  {
    class UniformBuffer : public NonCopyable
    {
    public:
      UniformBuffer(std::string _name);
      ~UniformBuffer();

      void Bind() const;
      void Unbind() const;

      void Reserve(uint _size);

      void setData(const void * _data, uint _size);
      void setData(const void * _data, uint _size, uint _offset);

      void setBind(GLuint _index);
      GLuint getBind() const;

      const std::string& getName() const;
   
    private:
      GLuint m_ubo;
      GLuint m_index;

      uint m_size;

      std::string m_name;
    };
  }
}

#endif //_ENGINE_GRAPHICS_UNIFORMBUFFER_H_
