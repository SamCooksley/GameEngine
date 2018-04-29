namespace engine
{
  namespace graphics
  {
    template <typename T>
    void BufferLayout::Add(const std::string & _name, uint _count, bool _normalized)
    {
      Add(_name, TypeData<T>::componentType, TypeData<T>::count, sizeof(T) * _count, _normalized);
    }
  }
}