#ifndef _ENGINE_UTILITIES_TIMER_H_
#define _ENGINE_UTILITIES_TIMER_H_

#include <chrono>

namespace engine {
namespace utilities {

    class Timer
    {
     private:
      using Clock = std::chrono::steady_clock;

     public:
      Timer();
      ~Timer();

      void Reset();

      float getSeconds()     const;
      float getMiliseconds() const;

     private:
      std::chrono::time_point<Clock> m_start;
    };

} } // engine::utilities

#endif //_ENGINE_UTILITIES_TIMER_H_