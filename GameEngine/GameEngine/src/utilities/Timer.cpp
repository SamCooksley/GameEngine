#include "stdafx.h"

#include "Timer.h"

#include <ctime>

namespace engine { namespace utilities {

  Timer::Timer()
  {
    Reset();
  }

  Timer::~Timer()
  { }

  void Timer::Reset()
  {
    m_start = std::clock();
  }

  double Timer::getSeconds() const
  {
    //get the tick difference and convert it to seconds.
    return (std::clock() - m_start) / double(CLOCKS_PER_SEC);
  }

  double Timer::getMiliseconds() const
  {
    //get the tick difference and convert it to miliseconds.
    return (std::clock() - m_start) / double(CLOCKS_PER_SEC) * 1000.0f;
  }

} }