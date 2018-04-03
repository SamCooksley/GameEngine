#ifndef _ENGINE_UTILITIES_IMAGE_H_
#define _ENGINE_UTILITIES_IMAGE_H_

#include "core\Types.h"

#include "File.h"

namespace engine
{
  namespace file
  {
    struct ImageData
    {
      uint width, height;
      std::vector<byte> pixels;
    };

    bool LoadImage(const std::string & _path, ImageData * _outData, bool _flipY = true);
    bool LoadImagePowerOf2(const std::string & _path, ImageData * _outData, bool _flipY = true);
  }
}

#endif //_ENGINE_UTILITIES_IMAGE_H_