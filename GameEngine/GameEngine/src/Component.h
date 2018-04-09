#ifndef _ENGINE_COMPONENT_H_
#define _ENGINE_COMPONENT_H_

#include "core\Object.h"

#include "graphics\Renderer.h"

namespace engine {

  class GameObject;

  class Component : public Object
  {
    friend class GameObject;
  public:
    virtual ~Component();

    std::shared_ptr<GameObject> getGameObject();
    std::shared_ptr<const GameObject> getGameObject() const;

    void setEnable(bool _enable);
    bool getEnabled() const;

    void Destroy();

  protected:
    Component();

    virtual void OnAwake();
    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnRender(graphics::Renderer & _renderer);

    virtual void OnEnable(bool _enable);

    virtual void OnDestroy();

  private:
    void Awake();
    void Update();
    void Render(graphics::Renderer & _renderer);

    std::weak_ptr<GameObject> m_gameObject;

    bool m_enabled;

    bool m_start;
    bool m_shouldDestroy;
  };

}

#endif //_ENGINE_COMPONENT_H_