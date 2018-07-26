#include "stdafx.h"

#include "LightBuffer.h"

namespace engine {
namespace graphics {

  const String LightBuffer::NAME = "Lights";

  const uint LightBuffer::MAX_DIRECTIONAL = 5u;
  const uint LightBuffer::MAX_POINT = 10u;
  const uint LightBuffer::MAX_SPOT = 5u;

  const uint LightBuffer::NUMDIR_OFFSET = 0u;
  const uint LightBuffer::NUMPOINT_OFFSET = 4u;
  const uint LightBuffer::NUMSPOT_OFFSET = 8u;

  const uint LightBuffer::AMBIENT_OFFSET = 16u;
  
  const uint LightBuffer::DIRECTIONAL_OFFSET = 32u;

  const uint LightBuffer::Directional::COLOUR_OFFSET = 0u;
  const uint LightBuffer::Directional::DIRECTION_OFFSET = 16u;
  const uint LightBuffer::Directional::SIZE = 32u;

  const uint LightBuffer::POINT_OFFSET = LightBuffer::DIRECTIONAL_OFFSET + LightBuffer::Directional::SIZE * LightBuffer::MAX_DIRECTIONAL;

  const uint LightBuffer::Point::COLOUR_OFFSET = 0u;
  const uint LightBuffer::Point::POSITION_OFFSET = 16u;
  const uint LightBuffer::Point::RADIUS_OFFSET = 32u;
  const uint LightBuffer::Point::SIZE = 48u;

  const uint LightBuffer::SPOT_OFFSET = LightBuffer::POINT_OFFSET + LightBuffer::Point::SIZE * LightBuffer::MAX_POINT;

  const uint LightBuffer::Spot::COLOUR_OFFSET = 0u;
  const uint LightBuffer::Spot::POSITION_OFFSET = 16u;
  const uint LightBuffer::Spot::DIRECTION_OFFSET = 32u;
  const uint LightBuffer::Spot::CUTOFF_OFFSET = 48u;
  const uint LightBuffer::Spot::OUTERCUTOFF_OFFSET = 52u;
  const uint LightBuffer::Spot::RADIUS_OFFSET = 64u;
  const uint LightBuffer::Spot::SIZE = 80u;

  const uint LightBuffer::SIZE = LightBuffer::SPOT_OFFSET + LightBuffer::Spot::SIZE * LightBuffer::MAX_SPOT;

  LightBuffer::LightBuffer() :
    UniformBuffer(LightBuffer::NAME)
  {
    Reserve(LightBuffer::SIZE);
  }
  
  LightBuffer::~LightBuffer()
  { }

  void LightBuffer::setLights(const Lights & _lights)
  {
    uint numDir = std::min(_lights.directional.size(), LightBuffer::MAX_DIRECTIONAL);
    uint numPoint = std::min(_lights.point.size(), LightBuffer::MAX_POINT);
    uint numSpot = std::min(_lights.spot.size(), LightBuffer::MAX_SPOT);

    std::vector<byte> data(LightBuffer::SIZE);

    memcpy(&data[LightBuffer::NUMDIR_OFFSET],   &numDir,   sizeof(int));
    memcpy(&data[LightBuffer::NUMPOINT_OFFSET], &numPoint, sizeof(int));
    memcpy(&data[LightBuffer::NUMSPOT_OFFSET],  &numSpot,  sizeof(int));

    memcpy(&data[LightBuffer::AMBIENT_OFFSET], glm::value_ptr(_lights.ambient), sizeof(glm::vec3));

    for (uint i = 0; i < numDir; ++i)
    {
      const DirectionalLight & dir = _lights.directional[i];
      uint offset = LightBuffer::DIRECTIONAL_OFFSET + LightBuffer::Directional::SIZE * i;

      memcpy(&data[offset + LightBuffer::Directional::COLOUR_OFFSET],    glm::value_ptr(dir.colour),    sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Directional::DIRECTION_OFFSET], glm::value_ptr(dir.direction), sizeof(glm::vec3));
    }

    for (uint i = 0; i < numPoint; ++i)
    {
      const PointLight & point = _lights.point[i];
      uint offset = LightBuffer::POINT_OFFSET + LightBuffer::Point::SIZE * i;
      
      memcpy(&data[offset + LightBuffer::Point::COLOUR_OFFSET],    glm::value_ptr(point.colour),   sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Point::POSITION_OFFSET],  glm::value_ptr(point.position), sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Point::RADIUS_OFFSET],    &point.radius,                  sizeof(float));
    }

    for (uint i = 0; i < numSpot; ++i)
    {
      const SpotLight & spot = _lights.spot[i];
      uint offset = LightBuffer::SPOT_OFFSET + LightBuffer::Spot::SIZE * i;

      memcpy(&data[offset + LightBuffer::Spot::COLOUR_OFFSET],      glm::value_ptr(spot.colour),    sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Spot::POSITION_OFFSET],    glm::value_ptr(spot.position),  sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Spot::DIRECTION_OFFSET],   glm::value_ptr(spot.direction), sizeof(glm::vec3));
      memcpy(&data[offset + LightBuffer::Spot::CUTOFF_OFFSET],      &spot.cutoff,                   sizeof(float));
      memcpy(&data[offset + LightBuffer::Spot::OUTERCUTOFF_OFFSET], &spot.outerCutoff,              sizeof(float));
      memcpy(&data[offset + LightBuffer::Spot::RADIUS_OFFSET],      &spot.radius,                   sizeof(float));
    }

    setData(data.data(), data.size());
  }

} } // engine::graphics