#include "stdafx.h"

#include "TestScene.h"

#include "GameObject.h"

#include "MeshRenderer.h"
#include "CameraMovement.h"
#include "Light.h"
#include "Resources.h"

#include "LoadObjects.h"

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
    light->setColour(glm::vec3(.5f, .5f, .5f) * 2.f);
    light->setPoint(1.f, 1.f);
    light->setDirectional();
    
    go->getComponent<Transform>()->setPosition(glm::vec3(2.f, 1.5f, -7.f)).setRotation(glm::quat(glm::vec3(-45.f, 0.f, 45.f))).setLocalScale(glm::vec3(0.1f));

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);

    auto sponza = Load("resources/sponza/sponza.obj");
    sponza->getComponent<Transform>()->setLocalScale(glm::vec3(.01f));
  }
}