#ifndef _ENGINE_GRAPHICS_UNIFORMBUFFERS_H_
#define _ENGINE_GRAPHICS_UNIFORMBUFFERS_H_

#include "UniformBuffer.h"

namespace engine
{
  namespace graphics
  {
    class UniformBuffers
    {
      friend class Shader;
    public:
      UniformBuffers();
      ~UniformBuffers();

      void Add(std::unique_ptr<UniformBuffer> _buffer);

      bool Exists(const std::string & _name) const;

      UniformBuffer & getBuffer(const std::string & _name);

    private:
      std::map<std::string, std::unique_ptr<UniformBuffer>> m_buffers;
    };
  }
}

#endif //_ENGINE_GRAPHICS_UNIFORMBUFFERS_H_