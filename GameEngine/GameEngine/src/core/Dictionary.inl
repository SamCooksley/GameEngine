namespace engine {

  template <typename key, typename value>
  Dictionary<key, value>::Dictionary()
  { }
  
  template <typename key, typename value>
  Dictionary<key, value>::~Dictionary()
  { }
  
  template <typename key, typename value>
  typename Dictionary<key, value>::iterator Dictionary<key, value>::begin()
  {
    return m_list.begin();
  }
  
  template <typename key, typename value>
  typename Dictionary<key, value>::iterator Dictionary<key, value>::end()
  {
    return m_list.end();
  }
  
  template <typename key, typename value>
  typename Dictionary<key, value>::const_iterator Dictionary<key, value>::begin() const
  {
    return m_list.begin();
  }
  
  template <typename key, typename value>
  typename Dictionary<key, value>::const_iterator Dictionary<key, value>::end() const
  {
    return m_list.end();
  }
  
  template <typename key, typename value>
  typename  Dictionary<key, value>::map_iterator Dictionary<key, value>::mbegin() const
  {
    return m_keyToIndex.begin();
  }
  
  template <typename key, typename value>
  typename  Dictionary<key, value>::map_iterator Dictionary<key, value>::mend() const
  {
    return m_keyToIndex.end();
  }
  
  template <typename key, typename value>
  value & Dictionary<key, value>::operator[](size_t _i)
  {
    return m_list[_i];
  }
  
  template <typename key, typename value>
  const value & Dictionary<key, value>::operator[](size_t _i) const
  {
    return m_list[_i];
  }
  
  template <typename key, typename value>
  size_t Dictionary<key, value>::size() const
  {
    return m_list.size();
  }
  
  template <typename key, typename value>
  typename Dictionary<key, value>::map_iterator Dictionary<key, value>::find(const key & _key) const
  {
    return m_keyToIndex.find(_key);
  }
  
  template <typename key, typename value>
  bool Dictionary<key, value>::exists(const key & _key) const
  {
    return find(_key) != m_keyToIndex.end();
  }
  
  template <typename key, typename value>
  void Dictionary<key, value>::add(const key & _key, const value & _value)
  {
    if (exists(_key))
    {
      throw std::invalid_argument("key already exists");
    }
  
    m_list.push_back(_value);
    m_keyToIndex[_key] = m_list.size() - 1;
  }
  
  template <typename key, typename value>
  void Dictionary<key, value>::remove(const key & _key)
  {
    auto iter = m_keyToIndex.find(_key);
    remove(iter);
  }
  
  template <typename key, typename value>
  void Dictionary<key, value>::remove(typename map_iterator & _iterator)
  {
    if (_iterator == m_keyToIndex.begin() ||
        _iterator == m_keyToIndex.end())
    {
      return;
    }
  
    size_t index = _iterator->second;
    m_list.erase(m_list.begin() + _iterator->second);
    m_keyToIndex.erase(_iterator);
  
    for (auto & i : m_keyToIndex)
    {
      if (i.second > index)
      {
        --i.second;
      }
    }
  }
  
  template <typename key, typename value>
  void Dictionary<key, value>::removeAt(size_t _i)
  {
    m_list.erase(m_list.begin() + _i);
    
    size_t index = 0;
    for (auto i = m_keyToIndex.begin(); i != m_keyToIndex.end();)
    {
      if (i.second == _i)
      {
        auto prev = i;
        ++i;
        m_keyToIndex.erase(prev);
        continue;
      }
  
      if (i.second > _i)
      {
        --i.second;
      }
  
      ++i;
    }
  }
  
  template <typename key, typename value>
  void Dictionary<key, value>::clear()
  {
    m_list.clear();
    m_keyToIndex.clear();
  }

} // engine