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
    camera->AddComponent<Camera>()->setPerspective(60.f, 1.0f, 0.01f, 10000.0f);
    camera->AddComponent<CameraMovement>();

    auto mesh = Resources::Load<graphics::Mesh>("resources/models/cube.obj");

    auto material = graphics::Material::Create(Graphics::getDefaultMaterial());

    material->setTexture("diffuse", graphics::Texture2D::Load("resources/textures/stone_wall/diffuse.jpg"));
    material->setTexture("normal", graphics::Texture2D::Load("resources/textures/stone_wall/normal.jpg"));
    material->setTexture("specular", graphics::Texture2D::Load("resources/textures/stone_wall/specular.jpg"));
    material->setTexture("displacement", graphics::Texture2D::Load("resources/textures/stone_wall/displacement.jpg"));

    auto go = GameObject::Instantiate();

    auto mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    mr->setMaterial(material);

    go->getComponent<Transform>()->setPosition(glm::vec3(0, 0, -5));

    
    material = graphics::Material::Create(material);

    material->setTexture("diffuse", graphics::Texture2D::Load("resources/textures/stone_wall_2/diffuse.jpg"));
    material->setTexture("normal", graphics::Texture2D::Load("resources/textures/stone_wall_2/normal.jpg"));
    material->setTexture("specular", graphics::Texture2D::Load("resources/textures/stone_wall_2/specular.jpg"));
    material->setTexture("displacement", graphics::Texture2D::Load("resources/textures/stone_wall_2/displacement.jpg"));
    material->setUniform("displacementScale", 0.1f);
    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    mr->setMaterial(material);

    go->getComponent<Transform>()->setPosition(glm::vec3(2, 0, -5));







    go = GameObject::Instantiate();

    auto light = go->AddComponent<Light>();
    light->setColour(glm::vec3(.5f, .5f, .5f) * 1.5f);
    light->setPoint(0.f, 0.f);
    //light->setDirectional();
    
    go->getComponent<Transform>()->setPosition(glm::vec3(0.f, 5.f, 0.f)).setRotation(glm::quat(glm::vec3(-45.f, 0.f, 45.f))).setLocalScale(glm::vec3(0.1f));

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);

    auto sponza = Load("resources/sponza/sponza.obj");
    sponza->getComponent<Transform>()->setLocalScale(glm::vec3(0.01f));
  }
}