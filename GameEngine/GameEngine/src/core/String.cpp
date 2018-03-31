#include "stdafx.h"

#include "String.h"

namespace engine
{
  namespace string
  {
    struct iequal
    {
      bool operator()(int _lhs, int _rhs) const
      {
        return tolower(_lhs) == tolower(_rhs);
      }
    };

    bool AreIEqual(const std::string & _lhs, const std::string & _rhs)
    {
      /*if (_lhs.size() != _rhs.size()) { return false; }

      size_t size = std::min(_lhs.size(), _rhs.size());
      for (size_t i = 0; i < size; ++i)
      {
        if (tolower(_lhs[i]) != tolower(_rhs[i]))
        {
          return false;
        }
      }

      return true;*/
      return std::equal(std::begin(_lhs), std::end(_lhs), std::begin(_rhs), std::end(_rhs), iequal());
    }
  }
}