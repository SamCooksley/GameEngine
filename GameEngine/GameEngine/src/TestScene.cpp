#include "stdafx.h"

#include "TestScene.h"

#include "GameObject.h"

#include "MeshRenderer.h"
#include "CameraMovement.h"

#include "Resources.h"

namespace engine
{
  TestScene::TestScene()
  { }

  TestScene::~TestScene()
  { }

  void TestScene::Init()
  {
    Scene::Init();

    auto camera = GameObject::Instantiate();
    camera->AddComponent<Camera>();
    camera->AddComponent<CameraMovement>();

    auto mesh = Resources::Load<graphics::Mesh>("resources/models/cube.obj");

    auto go = GameObject::Instantiate();

    auto mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);

    go->getComponent<Transform>()->setPosition(glm::vec3(0, 0, -5));
  }
}