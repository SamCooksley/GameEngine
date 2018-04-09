#include "stdafx.h"

#include "Timer.h"

namespace engine 
{
  namespace utilities
  {
    Timer::Timer()
    {
      Reset();
    }

    Timer::~Timer()
    { }

    void Timer::Reset()
    {
      m_start = Clock::now();
    }

    float Timer::getSeconds() const
    {
      std::chrono::duration<float> dur = Clock::now() - m_start;
      return dur.count();
    }

    float Timer::getMiliseconds() const
    {
      std::chrono::duration<float, std::milli> dur = Clock::now() - m_start;
      return dur.count();
    }
  }
}