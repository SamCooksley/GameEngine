#include "stdafx.h"

#include "Indices.h"

namespace engine
{
  namespace graphics
  {
    Indices::Indices() :
      m_indices(), m_max(0u)
    { }

    Indices::Indices(uint32 * _indices, size_t _size) :
      m_indices(_indices, _indices + _size), m_max(0u)
    {
      UpdateMax();
    }

    Indices::Indices(std::vector<uint32> _indices) :
      m_indices(std::move(_indices)), m_max(0u)
    {
      UpdateMax();
    }

    Indices::~Indices()
    { }

    void Indices::add(uint32 _index)
    {
      m_indices.push_back(_index);

      m_max = std::max(m_max, _index);
    }

    size_t Indices::size() const
    {
      return m_indices.size();
    }

    bool Indices::empty() const
    {
      return m_indices.empty();
    }

    uint32 Indices::operator[](size_t _i) const
    {
      return m_indices[_i];
    }

    std::vector<uint32>::const_iterator Indices::begin() const
    {
      return m_indices.begin();
    }

    std::vector<uint32>::const_iterator Indices::end() const
    {
      return m_indices.end();
    }

    void Indices::setIndices(std::vector<uint32> _indices)
    {
      m_indices = std::move(_indices);
      UpdateMax();
    }

    bool Indices::CanUseShort() const
    {
      return m_max < static_cast<uint32>(std::numeric_limits<uint16>().max());
    }

    bool Indices::CanUseByte() const
    {
      return m_max < static_cast<uint32>(std::numeric_limits<uint8>().max());
    }

    IndexType Indices::getSmallestType() const
    {
      return CanUseByte() ? IndexType::UNSIGNED_BYTE : 
                CanUseShort() ? IndexType::UNSIGNED_SHORT : 
                    IndexType::UNSIGNED_INT;
    }

    std::vector<uint32> Indices::ToInt() const
    {
      return m_indices;
    }

    std::vector<uint16> Indices::ToShort() const
    {
      if (!CanUseShort())
      {
        throw std::range_error("Cannot convert to uint16");
      }

      std::vector<uint16> indices(m_indices.size());

      for (size_t i = 0u; i < indices.size(); ++i)
      {
        indices[i] = static_cast<uint16>(m_indices[i]);
      }

      return std::move(indices);
    }

    std::vector<uint8> Indices::ToByte() const
    {
      if (!CanUseByte())
      {
        throw std::range_error("Cannot convert to uint8");
      }

      std::vector<uint8> indices(m_indices.size());

      for (size_t i = 0u; i < indices.size(); ++i)
      {
        indices[i] = static_cast<uint8>(m_indices[i]);
      }

      return std::move(indices);
    }

    std::pair<IndexType, std::vector<byte>> Indices::get() const
    {
      IndexType type = getSmallestType();

      std::vector<byte> data;

      if (type == IndexType::UNSIGNED_BYTE)
      {
        std::vector<uint8> i = ToByte();
        data.resize(m_indices.size() * sizeof(uint8));
        memcpy(&data[0], &i[0], data.size());
      }
      else if (type == IndexType::UNSIGNED_SHORT)
      {
        std::vector<uint16> i = ToShort();
        data.resize(m_indices.size() * sizeof(uint16));
        memcpy(&data[0], &i[0], data.size());
      }
      else
      {
        std::vector<uint32> i = ToInt();
        data.resize(m_indices.size() * sizeof(uint32));
        memcpy(&data[0], &i[0], data.size());
      }

      return std::move(std::make_pair(type, std::move(data)));
    }

    void Indices::UpdateMax()
    {
      m_max = 0u;
      for (const uint32 & index : m_indices)
      {
        m_max = std::max(m_max, index);
      }
    }
  }
}