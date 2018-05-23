namespace engine
{
  namespace graphics
  {
    template <typename T>
    void Mesh::AddAttribute(
      const T * _data, uint _count,
      bool _normalized
    )
    {
      AddAttribute(_data, _count * sizeof(T), _count, TypeData<T>::type, TypeData<T>::component, TypeData<T>::count, _normalized);
    }
  }
}