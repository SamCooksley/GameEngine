#include "stdafx.h"

#include "String.h"

namespace engine {
namespace string {

  struct iequal
  {
    bool operator()(int _lhs, int _rhs) const
    {
      return tolower(_lhs) == tolower(_rhs);
    }
  };
  
  bool AreIEqual(const String & _lhs, const String & _rhs)
  {
    return std::equal(std::begin(_lhs), std::end(_lhs), std::begin(_rhs), std::end(_rhs), iequal());
  }
  
  std::vector<String> SplitStringWhitespace(const String & _input)
  {
    return SplitString(_input, " \t\n\f\r\v");
  }
  
  std::vector<String> SplitString(const String & _input, char _separator)
  {
    return SplitString(_input, String(&_separator, 1));
  }
  
  std::vector<String> SplitString(const String & _input, const String & _separator)
  {
    std::vector<String> output;

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
        output.push_back(_input.substr(last, i - last));
      }
      else if (wasSep)
      {
        last = i;
      }
  
      wasSep = isSep;
    }
  
    if (!wasSep)
    {
      output.push_back(_input.substr(last, _input.size() - last));
    }

    return output;
  }
 
} } // engine::string