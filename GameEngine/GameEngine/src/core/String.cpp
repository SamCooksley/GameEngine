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
      return std::equal(std::begin(_lhs), std::end(_lhs), std::begin(_rhs), std::end(_rhs), iequal());
    }

    void SplitStringWhitespace(const std::string & _input, std::vector<std::string> & _output)
    {
      SplitString(_input, " \t\n\f\r\v", _output);
    }

    void SplitString(const std::string & _input, char _separator, std::vector<std::string> & _output)
    {
      SplitString(_input, std::string(&_separator, 1), _output);
    }

    void SplitString(const std::string & _input, const std::string & _separator, std::vector<std::string> & _output)
    {
      size_t last = 0;
      bool wasSep = true;
      for (size_t i = 0; i < _input.size(); ++i)
      {
        bool isSep = false;
        for (char s : _separator)
        {
          if (_input[i] == s) { isSep = true; break; }
        }

        if (!wasSep && isSep)
        {
          _output.push_back(_input.substr(last, i - last));
        }
        else if (wasSep)
        {
          last = i;
        }

        wasSep = isSep;
      }

      if (!wasSep)
      {
        _output.push_back(_input.substr(last, _input.size() - last));
      }
    }
  }
}