#ifndef _ENGINE_TIME_H_
#define _ENGINE_TIME_H_

namespace engine
{
  class Time
  {
  public:
    Time() = delete;

    static float getDeltaTime();
  };
}

#endif //_ENGINE_TIME_H_