#ifndef _ENGINE_CORE_STRING_H_
#define _ENGINE_CORE_STRING_H_

#include <string>

namespace engine
{
  namespace string
  {
    bool AreIEqual(const std::string & _lhs, const std::string & _rhs);

    void SplitStringWhitespace(const std::string & _input, std::vector<std::string> & _output);
    void SplitString(const std::string & _input, char _separator, std::vector<std::string> & _output);
    void SplitString(const std::string & _input, const std::string & _separators, std::vector<std::string> & _output);  
  }
}

#endif //_ENGINE_CORE_STRING_H_