#include "stdafx.h"

#include "Component.h"

#include "GameObject.h"

namespace engine
{
  Component::Component()
  { }

  Component::~Component()
  { }

  void Component::OnAwake()
  { }

  void Component::OnStart()
  { }

  void Component::OnUpdate()
  { }

  void Component::OnRender()
  { }

  void Component::OnEnable(bool _enable)
  { }

  void Component::OnDestroy()
  {
    setEnable(false);
  }

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

  void Component::Render()
  {
    if (m_enabled)
    {
      OnRender();
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

  void Component::Destroy()
  {
    m_shouldDestroy = true;
  }
}