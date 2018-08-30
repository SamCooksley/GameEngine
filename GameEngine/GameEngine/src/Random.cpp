#include "stdafx.h"

#include "Random.h"

namespace engine {

  std::default_random_engine Random::s_engine;

  void Random::Init()
  {
    std::random_device rd;
    s_engine.seed(rd());
  }

  int Random::Range(int _min, int _max)
  {
    static std::uniform_int_distribution<int> d;
    using range = decltype(d)::param_type;
    return d(s_engine, range{ _min, _max });
  }

  float Random::Range(float _min, float _max)
  {
    static std::uniform_real_distribution<float> d;
    using range = decltype(d)::param_type;
    return d(s_engine, range{ _min, _max });
  }

  float Random::Value()
  {
    return Range(0.f, 1.f);
  }

} // engine