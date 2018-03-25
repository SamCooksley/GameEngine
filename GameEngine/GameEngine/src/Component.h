#ifndef _ENGINE_COMPONENT_H_
#define _ENGINE_COMPONENT_H_

#include "core\Object.h"

namespace engine {

  class GameObject;

  class Component : public core::Object
  {
    friend class GameObject;
  public:

  protected:
    void OnAwake();
    void OnStart();
    void OnUpdate();
    void OnRender();

  private:
    void Awake();
    void Update();
    void Render();

    std::weak_ptr<GameObject> m_gameObject;

    bool m_start;
  };

}

#endif //_ENGINE_COMPONENT_H_