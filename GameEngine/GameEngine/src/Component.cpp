#include "stdafx.h"

#include "Component.h"

#include "GameObject.h"

namespace engine {

  Component::Component()
  { }

  Component::~Component()
  { }

  std::shared_ptr<GameObject> Component::getGameObject()
  {
    return m_gameObject.lock();
  }

  std::shared_ptr<const GameObject> Component::getGameObject() const
  {
    return m_gameObject.lock();
  }

  void Component::OnAwake()
  { }

  void Component::OnStart()
  { }

  void Component::OnUpdate()
  { }

  void Component::OnRender(graphics::Renderer & _renderer)
  { }

  void Component::OnEnable(bool _enable)
  { }

  void Component::OnDestroy()
  { }

  void Component::Awake()
  {
    m_start = false;
    m_shouldDestroy = false;
    m_enabled = true;
    m_shouldDestroy = false;

    OnAwake();
  }

  void Component::Update()
  {
    if (m_enabled)
    {
      if (!m_start)
      {
        m_start = true;
        OnStart();
      }
      OnUpdate();
    }
  }

  void Component::Render(graphics::Renderer & _renderer)
  {
    if (m_enabled)
    {
      OnRender(_renderer);
    }
  }

  void Component::setEnable(bool _enable)
  {
    if (m_enabled != _enable)
    {
      m_enabled = _enable;

      if (m_gameObject.lock()->m_enabled)
      {
        OnEnable(m_enabled);
      }
    }
  }

  bool Component::getEnabled() const
  {
    return m_enabled;
  }

  void Component::Destroy()
  {
    m_shouldDestroy = true;
  }

} // engine