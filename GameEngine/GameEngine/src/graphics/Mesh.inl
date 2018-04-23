namespace engine
{
  namespace graphics
  {
    template <typename T>
    void Mesh::setAttribute(
      const std::string & _name, 
      const T * _data, uint _count,
      bool _normalized
    )
    {
      setAttribute(_name, _data, _count * sizeof(T), _count, TypeData<T>::type, _normalized);
    }
  }
}