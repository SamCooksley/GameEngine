#ifndef _ENGINE_GRAPHICS_INDICES_H_
#define _ENGINE_GRAPHICS_INDICES_H_

#include "IndexBuffer.h"

namespace engine
{
  namespace graphics
  {
    class Indices
    {
    public:
      Indices();
      Indices(uint32 * _indices, size_t _size);
      Indices(std::vector<uint32> _indices);

      ~Indices();

      void add(uint32 _index);

      size_t size() const;
      bool empty() const;

      uint32 operator[](size_t _i) const;

      std::vector<uint32>::const_iterator begin() const;
      std::vector<uint32>::const_iterator end() const;

      void setIndices(std::vector<uint32> _indices);

      bool CanUseShort() const;
      bool CanUseByte() const;

      IndexType getSmallestType() const;

      std::vector<uint32> ToInt() const;
      std::vector<uint16> ToShort() const;
      std::vector<uint8> ToByte() const;

      std::pair<IndexType, std::vector<byte>> get() const;

    private:
      void UpdateMax();

      std::vector<uint32> m_indices;
      uint32 m_max;
    };
  }
}

#endif //_ENGINE_GRAPHICS_INDICES_H_