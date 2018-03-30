#ifndef _ENGINE_UTILITIES_FILE_H_
#define _ENGINE_UTILITIES_FILE_H_

#include <fstream>

namespace engine 
{ 
  namespace file 
  {  
    std::string getDirectory(const std::string & _path);
    std::string getFilename(const std::string & _path);
    std::string getFilenameWithoutExtension(const std::string & _path);
    std::string getFileExtension(const std::string & _path);
  }
}

#endif //_ENGINE_UTILITIES_FILE_H_
