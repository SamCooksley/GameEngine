#ifndef _ENGINE_COMPONENT_H_
#define _ENGINE_COMPONENT_H_

#include "core\Object.h"

namespace engine {

  class GameObject;

  class Component : public core::Object
  {
    friend class GameObject;
  public:
    virtual ~Component();

    void setEnable(bool _enable);

    void Destroy();

  protected:
    Component();

    virtual void OnAwake();
    virtual void OnStart();
    virtual void OnUpdate();
    virtual void OnRender();

    virtual void OnEnable(bool _enable);

    virtual void OnDestroy();

  private:
    void Awake();
    void Update();
    void Render();

    std::weak_ptr<GameObject> m_gameObject;

    bool m_enabled;

    bool m_start;
    bool m_shouldDestroy;
  };

}

#endif //_ENGINE_COMPONENT_H_