#include "stdafx.h"

#include "Component.h"

namespace engine
{
  void Component::OnAwake()
  { }

  void Component::OnStart()
  { }

  void Component::OnUpdate()
  { }

  void Component::OnRender()
  { }

  void Component::Awake()
  {
    m_start = false;
    OnAwake();
  }

  void Component::Update()
  {
    if (!m_start)
    {
      OnStart();
    }
    Update();
  }

  void Component::Render()
  {
    OnRender();
  }
}