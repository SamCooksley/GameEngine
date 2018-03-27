#ifndef _ENGINE_UTILITIES_TIMER_H_
#define _ENGINE_UTILITIES_TIMER_H_

namespace engine { namespace utilities {

  class Timer
  {
  public:
    Timer();
    ~Timer();

    void Reset();

    double getSeconds()     const; 
    double getMiliseconds() const;

  private:
    long m_start; 
  };

} }

#endif //_ENGINE_UTILITIES_TIMER_H_