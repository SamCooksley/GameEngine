#include "stdafx.h"

#include "LightBuffer.h"

namespace engine
{
  namespace graphics
  {
    const std::string LightBuffer::name = "Lights";

    const uint LightBuffer::max_lights = 10;

    const uint LightBuffer::type_offset        = 0;
    const uint LightBuffer::position_offset    = 32;
    const uint LightBuffer::direction_offset   = 48;
    const uint LightBuffer::colour_offset      = 64;
    const uint LightBuffer::linear_offset      = 4;
    const uint LightBuffer::quadratic_offset   = 8;
    const uint LightBuffer::cutoff_offset      = 12;
    const uint LightBuffer::outercutoff_offset = 16;

    const uint LightBuffer::light_size = 80;

    const uint LightBuffer::lights_size = LightBuffer::light_size * LightBuffer::max_lights;

    const uint LightBuffer::ambient_offset = LightBuffer::lights_size;

    const uint LightBuffer::size = LightBuffer::lights_size + sizeof(glm::vec4);

    std::unique_ptr<UniformBuffer> LightBuffer::Create()
    {
      auto buffer = std::make_unique<graphics::UniformBuffer>(graphics::LightBuffer::name);
      buffer->Reserve(graphics::LightBuffer::size);
      buffer->Unbind();
      return std::move(buffer);
    }

    void LightBuffer::setLight(UniformBuffer & _buffer, const Light & _light, uint _num)
    {
      std::vector<byte> data(LightBuffer::light_size);

      memcpy(&data[LightBuffer::type_offset], &_light.type, sizeof(LightType));
      memcpy(&data[LightBuffer::position_offset], glm::value_ptr(_light.position), sizeof(glm::vec3));
      memcpy(&data[LightBuffer::direction_offset], glm::value_ptr(_light.direction), sizeof(glm::vec3));
      memcpy(&data[LightBuffer::colour_offset], glm::value_ptr(_light.colour), sizeof(glm::vec3));
      memcpy(&data[LightBuffer::linear_offset], &_light.linear, sizeof(float));
      memcpy(&data[LightBuffer::quadratic_offset], &_light.quadratic, sizeof(float));
      memcpy(&data[LightBuffer::cutoff_offset], &_light.cutoff, sizeof(float));
      memcpy(&data[LightBuffer::outercutoff_offset], &_light.outerCutoff, sizeof(float));

      _buffer.Bind();
      _buffer.setData(&data[0], LightBuffer::light_size, LightBuffer::light_size * _num);
      _buffer.Unbind();
    }

    void LightBuffer::setAmbient(UniformBuffer & _buffer, const glm::vec3 & _ambient)
    {
      _buffer.Bind();
      _buffer.setData(glm::value_ptr(_ambient), sizeof(glm::vec3), LightBuffer::ambient_offset);
      _buffer.Unbind();
    }
  }
}