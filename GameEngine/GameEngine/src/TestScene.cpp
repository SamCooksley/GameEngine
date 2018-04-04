#include "stdafx.h"

#include "TestScene.h"

#include "GameObject.h"

#include "MeshRenderer.h"
#include "CameraMovement.h"
#include "Light.h"
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

    go = GameObject::Instantiate();

    auto light = go->AddComponent<Light>();
    light->setColour(glm::vec3(.05f, .1f, .05f));
    light->setPoint(0, 0);
    //light->setDirectional();

    go->getComponent<Transform>()->setPosition(glm::vec3(5, 3, -10));
    go->getComponent<Transform>()->setRotation(glm::quat(glm::vec3(-45.f, 0.f, 45.f)));
  }
}