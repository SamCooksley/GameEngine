#include "stdafx.h"

#include "File.h"

namespace engine {
namespace file {

  static size_t getDirectoryEndLocation(const String & _path)
  {
    return _path.find_last_of("\\/") + 1;
  }
  
  static size_t getFileExtensionStartLocation(const String & _path)
  {
    return _path.find_last_of(".");
  }
  
  String getDirectory(const String & _path)
  {
    size_t pos = getDirectoryEndLocation(_path);
    return (pos != String::npos) ? _path.substr(0, pos) : "";
  }
  
  String getFilename(const String & _path)
  {
    size_t pos = getDirectoryEndLocation(_path);
    if (pos == String::npos) { pos = 0; }
    return _path.substr(pos, _path.size() - pos);
  }
  
  String getFilenameWithoutExtension(const String & _path)
  {
    size_t pos = getDirectoryEndLocation(_path);
    if (pos == String::npos) { pos = 0; }
    size_t end = getFileExtensionStartLocation(_path);
    if (end == String::npos) { end = _path.size(); }
    return _path.substr(pos, end - pos);
  }
  
  String getFileExtension(const String & _path)
  {
    size_t pos = getFileExtensionStartLocation(_path);
    return (pos != String::npos) ? _path.substr(pos, _path.size() - pos) : "";
  }
  
} } // engine::file