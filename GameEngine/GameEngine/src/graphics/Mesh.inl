namespace engine
{
  namespace graphics
  {
    template <typename T>
    void Mesh::setAttribute(const std::string & _name, const T * _data, uint _count)
    {
      setAttribute(_name, _data, _count * sizeof(T), TypeData<T>::component, TypeData<T>::count);
    }
  }
}