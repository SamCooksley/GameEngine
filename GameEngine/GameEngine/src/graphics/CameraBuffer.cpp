#include "stdafx.h"

#include "CameraBuffer.h"

namespace engine
{
  namespace graphics
  {
    const std::string CameraBuffer::name = "Camera";

    const uint CameraBuffer::projection_offset = 0;
    const uint CameraBuffer::view_offset = 64;
    const uint CameraBuffer::vp_offset = 128;
    const uint CameraBuffer::position_offset = 192;

    const uint CameraBuffer::size = 208;

    std::unique_ptr<UniformBuffer> CameraBuffer::Create()
    {
      auto buffer = std::make_unique<graphics::UniformBuffer>(graphics::CameraBuffer::name);
      buffer->Reserve(graphics::CameraBuffer::size);
      buffer->Unbind();
      return std::move(buffer);
    }

    void CameraBuffer::setCamera(UniformBuffer & _buffer, const Camera & _camera)
    {
      std::vector<byte> data(CameraBuffer::size);

      memcpy(&data[CameraBuffer::projection_offset], glm::value_ptr(_camera.projection), sizeof(glm::mat4));
      memcpy(&data[CameraBuffer::view_offset], glm::value_ptr(_camera.view), sizeof(glm::mat4));
      memcpy(&data[CameraBuffer::vp_offset], glm::value_ptr(_camera.vp), sizeof(glm::mat4));
      memcpy(&data[CameraBuffer::position_offset], glm::value_ptr(_camera.position), sizeof(glm::vec3));

      _buffer.Bind();
      _buffer.setData(&data[0], CameraBuffer::size);
      _buffer.Unbind();
    }
  }
}