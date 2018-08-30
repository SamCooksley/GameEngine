#include "stdafx.h"

#include "TextureNoiseGenerator.h"

#include "Random.h"

namespace engine {
namespace graphics {

  std::unique_ptr<Texture2D> TextureNoiseGenerator::GenerateNoise2D(TextureFormat _format, int _width, int _height, float _min, float _max) const
  {
    TextureBaseFormat format = TextureFormatBase(_format);
    int componentCount = TextureBaseFormatToChannelCount(format);

    std::vector<float> data(_width * _height * componentCount);

    for (auto & value : data)
    {
      value = Random::Range(_min, _max);
    }

    auto texture = std::make_unique<Texture2D>(_format, _width, _height, 1);
    texture->setData(format, TextureDataType::FLOAT, data.data(), data.size());

    texture->setFilter(TextureFilter::NEAREST);
    texture->setWrap(TextureWrap::REPEAT);

    return texture;
  }

} } // engine::graphics
