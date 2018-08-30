#ifndef _ENGINE_RANDOM_H_
#define _ENGINE_RANDOM_H_

namespace engine {

  class Random
  {
  public:
    static void Init();

    static int Range(int _min, int _max);
    static float Range(float _min, float _max);

    static float Value();

  private:
    static std::default_random_engine s_engine;

  public:
    Random() = delete;
  };

} // engine

#endif // _ENGINE_RANDOM_H_