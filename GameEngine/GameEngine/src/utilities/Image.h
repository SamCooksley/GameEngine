#ifndef _ENGINE_UTILITIES_IMAGE_H_
#define _ENGINE_UTILITIES_IMAGE_H_

#include "core\Types.h"

#include "File.h"

namespace engine {
namespace file {

  struct ImageData
  {
    int width, height;
    std::vector<byte> pixels;
  };
  
  bool LoadImage(const String & _path, ImageData * _outData, bool _flipY = true);
  bool LoadImagePowerOf2(const String & _path, ImageData * _outData, bool _flipY = true);
 
} } // engine::file

#endif //_ENGINE_UTILITIES_IMAGE_H_