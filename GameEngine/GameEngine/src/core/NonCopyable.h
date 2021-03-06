#ifndef _ENGINE_CORE_NONCOPYABLE_H_
#define _ENGINE_CORE_NONCOPYABLE_H_

namespace engine {

  class NonCopyable
  {
   public:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
  };

} // engine

#endif //_ENGINE_CORE_NONCOPYABLE_H_