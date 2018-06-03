#ifndef _ENGINE_UTILITIES_FILE_H_
#define _ENGINE_UTILITIES_FILE_H_

#include <fstream>

#include "core\Types.h"

namespace engine { 
namespace file {  
    
  String getDirectory(const String & _path);
  String getFilename(const String & _path);
  String getFilenameWithoutExtension(const String & _path);
  String getFileExtension(const String & _path);
 
} } // engine::file

#endif //_ENGINE_UTILITIES_FILE_H_
