namespace engine
{
  namespace graphics
  {
    template <typename T>
    void BufferLayout::Add(uint _count, bool _normalized)
    {
      Add(TypeData<T>::componentType, TypeData<T>::count, sizeof(T) * _count, _normalized);
    }
  }
}