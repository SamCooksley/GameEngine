#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

#include "core\Object.h"

#include "GameObject.h"

namespace engine {

  class Scene : public Object
  {
    friend class Application;
    friend class GameObject;

   public:
    template <class T>
    static void SetScene();

    ~Scene();

    void Clear();

   protected:
    Scene();
    virtual void Init();

   private:
    void Add(const std::shared_ptr<GameObject> & _gameObject);

    void Update();
    void Render(graphics::Renderer & _renderer);

   private:
    std::vector<std::shared_ptr<GameObject>> m_gameObjects;
  };

} // engine

#include "Scene.inl"

#endif //_ENGINE_SCENE_H_
