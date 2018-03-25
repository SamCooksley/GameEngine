#ifndef _ENGINE_CORE_SHAREDFROMTHIS_H_
#define _ENGINE_CORE_SHAREDFROMTHIS_H_

namespace engine { namespace core {

  class Object;
  
  template <class T>
  class enable_shared_from_this : public virtual std::enable_shared_from_this<Object>
  {
  public:
    std::shared_ptr<T> getShared() { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }
    std::shared_ptr<const T> getShared() const { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }
  };

} }

#endif //_ENGINE_CORE_SHAREDFROMTHIS_H_