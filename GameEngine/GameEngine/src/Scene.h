#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

#include "GameObject.h"

namespace engine {

  class Scene
  {
    friend class Application;
    friend class GameObject;
  public:
    Scene();
    ~Scene();

    void Clear();

  protected:
    virtual void Init();

  private:
    void Add(std::shared_ptr<GameObject> _gameObject);

    void Update();
    void Render();

    std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  };

}

#endif //_ENGINE_SCENE_H_
