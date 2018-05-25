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

      template <class T>
      T * getBuffer()
      {
        static_assert(std::is_base_of<UniformBuffer, T>::value, "Object must be type of UniformBuffer");

        for (auto & buffer : m_buffers)
        {
          T * b = dynamic_cast<T*>(buffer.second.get());
          if (b != nullptr)
          {
            return b;
          }
        }

        return nullptr;
      }

      UniformBuffer * getBuffer(const std::string & _name);

    private: 
      std::map<std::string, std::unique_ptr<UniformBuffer>> m_buffers;
    };
  }
}

#endif //_ENGINE_GRAPHICS_UNIFORMBUFFERS_H_