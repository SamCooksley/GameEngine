#ifndef _ENGINE_CORE_OBJECT_H_
#define _ENGINE_CORE_OBJECT_H_

#include "Types.h"

namespace engine { namespace core {

  class Object : public std::enable_shared_from_this<Object>
  {
  public:
    virtual ~Object();

    const std::string & getName() const;
    void setName(const std::string & _name);

  protected:
    Object();

  private:
    std::string m_name;
  };

  bool operator==(const Object & _lhs, const Object & _rhs);
  bool operator!=(const Object & _lhs, const Object & _rhs);

} }

#define ENGINE_SETUPSHARED(x) \
  public: \
    inline std::shared_ptr<x> getShared()       { \
      static_assert(std::is_base_of<Object, x>::value, "Not of type Object"); \
      return std::static_pointer_cast<x>(this->shared_from_this()); } \
 \
    inline std::shared_ptr<const x> getShared() const { \
      static_assert(std::is_base_of<Object, x>::value, "Not of type Object"); \
      return std::static_pointer_cast<const x>(this->shared_from_this()); }

#endif //_ENGINE_CORE_OBJECT_H_