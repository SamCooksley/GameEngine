#include "stdafx.h"

#include "File.h"

namespace engine 
{
  namespace file
  {
    static size_t getDirectoryEndLocation(const std::string & _path)
    {
      return _path.find_last_of("\\/") + 1;
    }

    static size_t getFileExtensionStartLocation(const std::string & _path)
    {
      return _path.find_last_of(".");
    }

    std::string getDirectory(const std::string & _path)
    {
      size_t pos = getDirectoryEndLocation(_path);
      return (pos != std::string::npos) ? _path.substr(0, pos) : "";
    }

    std::string getFilename(const std::string & _path)
    {
      size_t pos = getDirectoryEndLocation(_path);
      if (pos == std::string::npos) { pos = 0; }
      return _path.substr(pos, _path.size() - pos);
    }

    std::string getFilenameWithoutExtension(const std::string & _path)
    {
      size_t pos = getDirectoryEndLocation(_path);
      if (pos == std::string::npos) { pos = 0; }
      size_t end = getFileExtensionStartLocation(_path);
      if (end == std::string::npos) { end = _path.size(); }
      return _path.substr(pos, end - pos);
    }

    std::string getFileExtension(const std::string & _path)
    {
      size_t pos = getFileExtensionStartLocation(_path);
      return (pos != std::string::npos) ? _path.substr(pos, _path.size() - pos) : "";
    }
  }
}