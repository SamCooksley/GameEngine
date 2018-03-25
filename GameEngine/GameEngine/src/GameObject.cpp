#include "stdafx.h"

#include "GameObject.h"

namespace engine
{
  std::shared_ptr<GameObject> GameObject::Create()
  {
    class EnableCreation : public GameObject { };

    auto go = std::make_shared<EnableCreation>();
    
    return go;
  }

  GameObject::GameObject() : 
    m_enabled(false)
  { }

  GameObject::~GameObject()
  { }

  void GameObject::Update()
  {
    if (m_enabled)
    {
      for (auto & component : m_components)
      {
        component->Update();
      }
    }
  }

  void GameObject::Render()
  {
    if (m_enabled)
    {
      for (auto & component : m_components)
      {
        component->Render();
      }
    }
  }
}