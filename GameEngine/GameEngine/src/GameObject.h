#ifndef _ENGINE_GAMEOBJECT_H_
#define _ENGINE_GAMEOBJECT_H_

#include "core\Object.h"

#include "Component.h"
#include "Transform.h"

namespace engine 
{
  class GameObject : public Object
  {
    friend class Component;
    friend class Scene;

  public:
    static std::shared_ptr<GameObject> Instantiate();
    static std::shared_ptr<GameObject> Create();

    ~GameObject();

    template <class T>
    inline std::shared_ptr<T> AddComponent();

    template <class T>
    inline std::shared_ptr<T> getComponent();
    
    template <class T> 
    inline std::shared_ptr<const T> getComponent() const;

    template <class T> 
    inline void getComponents(std::vector<std::shared_ptr<T>> & _outComponents);

    void setEnable(bool _enable);

    void Destroy();

  protected:
    GameObject();

  private:
    void Update();
    void Render(graphics::Renderer & _renderer);

    void OnDestroy();

    template <class T>
    std::shared_ptr<T> AddComponentInternal();

    template <class T>
    std::shared_ptr<T> getComponentInternal();

    template <class T>
    std::shared_ptr<const T> getComponentInternal() const;

    template <class T>
    void getComponentsInternal(std::vector<std::shared_ptr<T>> & _outComponents);

    bool m_enabled;
    bool m_shouldDestroy;

    std::vector<std::shared_ptr<Component>> m_components;

    std::weak_ptr<Transform> m_transform;

    ENGINE_SETUPSHARED(GameObject);
  };
}

#include "GameObject.inl"

#endif //_ENGINE_GAMEOBJECT_H_
