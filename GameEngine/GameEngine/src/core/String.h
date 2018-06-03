#ifndef _ENGINE_CORE_STRING_H_
#define _ENGINE_CORE_STRING_H_

#include <string>

namespace engine {

using String = std::string;

namespace string {
  
  bool AreIEqual(const String & _lhs, const String & _rhs);
  
  std::vector<String> SplitStringWhitespace(const String & _input);
  std::vector<String> SplitString(const String & _input, char _separator);
  std::vector<String> SplitString(const String & _input, const String & _separators);
  
} } // engine::string

#endif //_ENGINE_CORE_STRING_H_