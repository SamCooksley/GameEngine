#include "stdafx.h"

#include "GameObject.h"
#include "Application.h"

namespace engine
{
  std::shared_ptr<GameObject> GameObject::Instantiate()
  {
    auto go = Create();
    Application::s_context->scene->Add(go->getShared());
    return go;
  }

  std::shared_ptr<GameObject> GameObject::Create()
  {
    class EnableCreation : public GameObject { };

    auto go = std::make_shared<EnableCreation>();
    
    return go;
  }

  GameObject::GameObject() : 
    m_enabled(true), m_shouldDestroy(false)
  { }

  GameObject::~GameObject()
  { }

  void GameObject::setEnable(bool _enable)
  {
    if (m_enabled != _enable)
    {
      m_enabled = _enable;
      for (auto & c : m_components)
      {
        if (c->m_enabled)
        {
          c->OnEnable(m_enabled);
        }
      }
    }
  }

  void GameObject::Destroy()
  {
    m_shouldDestroy = true;
  }

  void GameObject::Update()
  {
    if (m_enabled)
    {
      for (auto & c : m_components)
      {
        c->Update();
      }
    }

    for (size_t i = 0; i < m_components.size();)
    {
      if (m_components[i]->m_shouldDestroy)
      {
        m_components[i]->OnDestroy();
        m_components.erase(m_components.begin() + i);
      }
      else { ++i; }
    }
  }

  void GameObject::Render()
  {
    if (m_enabled)
    {
      for (auto & c : m_components)
      {
        c->Render();
      }
    }
  }

  void GameObject::OnDestroy()
  {
    //setEnable(false);

    for (auto & c : m_components)
    {
      c->OnDestroy();
    }
    m_components.clear();
  }
}