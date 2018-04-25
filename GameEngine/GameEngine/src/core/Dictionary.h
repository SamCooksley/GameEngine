#ifndef _ENGINE_CORE_DICTIONARY_H_
#define _ENGINE_CORE_DICTIONARY_H_

namespace engine
{
  //Use a map for key-based element access but also keep
  //the iteration speed of vector array.
  template <typename key, typename value>
  class Dictionary
  {
  public:
    using list = std::vector<value>;
    using map = std::unordered_map<key, size_t>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;
    using map_iterator = typename map::const_iterator;

    Dictionary();
    ~Dictionary();

    typename iterator begin();
    typename iterator end();

    typename const_iterator begin() const;
    typename const_iterator end() const;

    typename map_iterator mbegin() const;
    typename map_iterator mend() const;

    value & operator[](size_t _i);
    const value & operator[](size_t _i) const;

    size_t size() const;

    typename map_iterator find(const key & _key) const;
    bool exists(const key & _key) const;

    void add(const key & _key, const value & _value);
    void remove(const key & _key);
    void remove(typename map_iterator & _iterator);

    void removeAt(size_t _i);

    void clear();

  private:
    list m_list;
    map m_keyToIndex;
  };
}

#include "Dictionary.inl"

#endif //_ENGINE_CORE_DICTIONARY_H_