#include "stdafx.h"

#include "RenderQueue.h"

namespace engine {
namespace graphics {

  const std::array<String, RenderQueue::COUNT> RenderQueue::s_names = {
    "deferred", "forward", "transparent"
  };
  
  const String & RenderQueue::ToString(Queue _queue)
  {
    if (_queue < 0 || _queue >= s_names.size())
    {
      throw std::invalid_argument("RenderQueue.ToString");
    }
  
    return s_names[_queue];
  }
  
  RenderQueue::Queue RenderQueue::FromString(const String & _s)
  {
    for (size_t i = 0u; i < s_names.size(); ++i)
    {
      if (string::AreIEqual(s_names[i], _s))
      {
        return static_cast<Queue>(i);
      }
    }
  
    throw std::invalid_argument("RenderQueue.FromString");
  }

} } // engine::graphics