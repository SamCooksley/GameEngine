#ifndef _ENGINE_CORE_NOTIMPLEMENTED_H_
#define _ENGINE_CORE_NOTIMPLEMENTED_H_

namespace engine {

  class not_implemented : public std::logic_error
  {
  public:
    not_implemented() :
      std::logic_error("function not implemented")
    { }
    not_implemented(const char * _message) : 
      std::logic_error(_message)
    { }
  };

} // engine

#endif // _ENGINE_CORE_NOTIMPLEMENTED_H_