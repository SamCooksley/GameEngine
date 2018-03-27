#include "stdafx.h"

#include "Scene.h"

namespace engine
{
  Scene::Scene()
  { }

  Scene::~Scene()
  {
    Clear();
  }

  void Scene::Clear()
  {
    for (auto & go : m_gameObjects)
    {
      go->OnDestroy();
    }
    m_gameObjects.clear();
  }

  void Scene::Init()
  { }

  void Scene::Add(std::shared_ptr<GameObject> _gameObject)
  {
    m_gameObjects.push_back(_gameObject);
  }

  void Scene::Update()
  {
    for (auto & go : m_gameObjects)
    {
      go->Update();
    }

    for (size_t i = 0; i < m_gameObjects.size();)
    {
      if (m_gameObjects[i]->m_shouldDestroy)
      {
        m_gameObjects[i]->OnDestroy();
        m_gameObjects.erase(m_gameObjects.begin() + i);
      }
      else { ++i; }
    }
  }

  void Scene::Render()
  {
    for (auto & go : m_gameObjects)
    {
      go->Render();
    }
  }
}