#ifndef _ENGINE_GAMEOBJECT_H_
#define _ENGINE_GAMEOBJECT_H_

#include "core\Object.h"

#include "Component.h"

namespace engine {

  class GameObject : public core::Object, public core::enable_shared_from_this<GameObject>
  {
  public:
    static std::shared_ptr<GameObject> Create();

    ~GameObject();

    template <class T>
    std::shared_ptr<T> AddComponent();

    template <class T>
    std::shared_ptr<T> getComponent();

    template <class T> 
    void getComponents(const std::vector<std::shared_ptr<T>> & _outComponents);

  protected:
    GameObject();

  private:
    void Update();
    void Render();

    template <class T>
    std::shared_ptr<T> AddComponentInternal();

    template <class T>
    std::shared_ptr<T> getComponentInternal();

    template <class T>
    void getComponentsInternal(const std::vector<std::shared_ptr<T>> & _outComponents);

    bool m_enabled;

    std::vector<std::shared_ptr<Component>> m_components;
  };
}

#include "GameObject.inl"

#endif //_ENGINE_GAMEOBJECT_H_
