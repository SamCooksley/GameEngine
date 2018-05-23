#ifndef _ENGINE_GRAPHICS_BUFFERLAYOUT_H_
#define _ENGINE_GRAPHICS_BUFFERLAYOUT_H_

#include "core\String.h"
#include "TypeData.h"

namespace engine
{
  namespace graphics
  {
    struct BufferElement
    {
      ComponentType type;
      uint count;
      uint size;
      bool normalized;
      uint offset;
    };

    class BufferLayout
    {
    public:
      BufferLayout() = default;
      ~BufferLayout() = default;

      const BufferElement & at(size_t _i) const;
      size_t getElementCount() const;

      uint getSize() const;

      template <typename T>
      void Add(uint _count, bool _normalized = false);

      void Add(ComponentType _type, uint _count, uint _size, bool _normalized = false);

    private:
      std::vector<BufferElement> m_elements;
      uint m_size;
    };
  }
}

#include "BufferLayout.inl"

#endif //_ENGINE_GRAPHICS_BUFFERLAYOUT_H_