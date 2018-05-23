#include "stdafx.h"

#include "BufferLayout.h"

namespace engine
{
  namespace graphics
  {
    const BufferElement & BufferLayout::at(size_t _i) const
    {
      return m_elements.at(_i);
    }

    size_t BufferLayout::getElementCount() const
    {
      return m_elements.size();
    }

    uint BufferLayout::getSize() const
    {
      return m_size;
    }

    void BufferLayout::Add(ComponentType _type, uint _count, uint _size, bool _normalized)
    {
      BufferElement element;
      element.type = _type;
      element.count = _count;
      element.size = _size;
      element.normalized = _normalized;
      element.offset = m_size;

      m_elements.push_back(element);

      m_size += _size;
    }
  }
}