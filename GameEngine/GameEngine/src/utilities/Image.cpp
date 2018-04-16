#include "stdafx.h"

#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

namespace engine
{
  namespace file
  {
    static uint NextPowerOfTwo(uint _input)
    {
      _input--;
      _input |= _input >> 1;
      _input |= _input >> 2;
      _input |= _input >> 4;
      _input |= _input >> 8;
      _input |= _input >> 16;
      _input++;

      return _input;
    }

    bool LoadImage(const std::string& _path, ImageData * _outData, bool _flipY)
    {
      if (_outData == nullptr) { return false; }

      stbi_set_flip_vertically_on_load(_flipY);

      int x, y, n; 
      unsigned char* pixels = stbi_load(_path.c_str(), &x, &y, &n, 4);
      if (pixels == nullptr) { return false; }

      _outData->width = x;
      _outData->height = y;

      uint size = x * y * 4;

      _outData->pixels.resize(size);
      memcpy(&_outData->pixels[0], pixels, size);

      stbi_image_free(pixels);

      return true;
    }

    bool LoadImagePowerOf2(const std::string& _path, ImageData* _outData, bool _flipY)
    {
      if (_outData == nullptr) { return false; }

      stbi_set_flip_vertically_on_load(_flipY);

      int w, h, n; 
      unsigned char* pixels = stbi_load(_path.c_str(), &w, &h, &n, 4);
      if (pixels == nullptr) { return false; }

      uint width = NextPowerOfTwo(w);
      uint height = NextPowerOfTwo(h);

      if (width == w && height == h)
      {
        _outData->width = w;
        _outData->height = h;

        uint size = w * h * 4;

        _outData->pixels.resize(size);
        memcpy(&_outData->pixels[0], pixels, size);
      }
      else
      {

        float scaleWidth = w / float(width);
        float scaleHeight = h / float(height);

        _outData->width = width;
        _outData->height = height;
        _outData->pixels.resize(width * height * 4);

        for (uint y = 0; y < height; ++y)
        {
          for (uint x = 0; x < width; ++x)
          {
            uint index = x * 4 + y * width * 4;
            uint other = int(x * scaleWidth) * 4 + int(y * scaleHeight) * w * 4;

            memcpy(&_outData->pixels[index], &pixels[other], 4);
          }
        }
      }

      stbi_image_free(pixels);

      return true;
    }
  }
}