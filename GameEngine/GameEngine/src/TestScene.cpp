#include "stdafx.h"

#include "TestScene.h"

#include "GameObject.h"

#include "MeshRenderer.h"
#include "CameraMovement.h"
#include "Light.h"
#include "Resources.h"

#include "LoadObjects.h"

#include "graphics\mesh\quad.h"

namespace engine {

  TestScene::TestScene()
  { }

  TestScene::~TestScene()
  { }

  void TestScene::Init()
  {
    Scene::Init();

    auto camera = GameObject::Instantiate();
    camera->AddComponent<Camera>()->setPerspective(60.f, .01f, 25.f);
    camera->AddComponent<CameraMovement>();

    auto mesh = Resources::Load<graphics::Mesh>("resources/models/cube.obj");
    
    auto material = std::make_shared<graphics::Material>(*Graphics::getContext().defaultMaterial);

    
    material->setTexture("albedo", graphics::Texture2D::Load("resources/textures/floor/albedo.png"));
    material->setTexture("normal", graphics::Texture2D::Load("resources/textures/floor/normal.png", true));
    //material->setTexture("metallicness", graphics::Texture2D::Load("resources/textures/floor/metallicness.png", true));
    material->setTexture("roughness", graphics::Texture2D::Load("resources/textures/floor/roughness.png", true));
    material->setTexture("height", graphics::Texture2D::Load("resources/textures/floor/displacement.png", true));
    material->setUniform("heightScale", .02f);
    material->setTexture("ao", graphics::Texture2D::Load("resources/textures/floor/ao.png", true));

    auto material2 = std::make_shared<graphics::Material>(*Graphics::getContext().defaultMaterial);
    material2->setTexture("albedo", graphics::Texture2D::Load("resources/textures/metal/albedo.png"));
    material2->setTexture("metallic", graphics::Texture2D::Load("resources/textures/metal/metallic.png"));
    material2->setTexture("roughness", graphics::Texture2D::Load("resources/textures/metal/roughness.png"));
    
    auto material3 = std::make_shared<graphics::Material>(*Graphics::getContext().defaultMaterial);
    material3->setTexture("albedo", graphics::Texture2D::Load("resources/textures/rock/albedo.png"));
    material3->setTexture("ao", graphics::Texture2D::Load("resources/textures/rock/ao.png"));
    material3->setTexture("height", graphics::Texture2D::Load("resources/textures/rock/height.png"));
    material3->setTexture("normal", graphics::Texture2D::Load("resources/textures/rock/normal.png"));
    material3->setTexture("roughness", graphics::Texture2D::Load("resources/textures/rock/roughness.png"));
    material3->setUniform("heighttScale", .02f);
    auto go = GameObject::Instantiate();

    go->AddComponent<MeshRenderer>()
      ->setMesh(mesh)
      .setMaterial(material);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0, 0, -5));

    auto quad = Resources::Load<graphics::Mesh>("resources/models/quad.obj");

    auto transShader = Resources::Load<graphics::Shader>("resources/shaders/pbr/transparent.glsl");
    auto transMat = std::make_shared<graphics::Material>(transShader);

    transMat->setTexture("albedo", graphics::Texture2D::Load("resources/textures/window/window.png"));
    transMat->setTexture("normal", std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(.5f, .5f, 1.f, 1.f)));
    transMat->setTexture("metallic", std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(0.f)));
    transMat->setTexture("roughness", std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(0.1f)));
    transMat->setTexture("ao", std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(1.f)));
    transMat->setTexture("height", graphics::Texture2D::Load("resources/textures/window/height.png", true));
    transMat->setUniform("heightScale", 0.1f);

    go = GameObject::Instantiate();

    go->AddComponent<MeshRenderer>()
      ->setMesh(quad)
      .setMaterial(transMat);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.f, .5f, 1.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-90.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(1.f));

    go = GameObject::Instantiate();

    go->AddComponent<MeshRenderer>()
      ->setMesh(quad)
      .setMaterial(transMat);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(-0.5f, 1.5f, 1.5f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-90.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(1.f));

    go = GameObject::Instantiate();

    go->AddComponent<MeshRenderer>()
      ->setMesh(quad)
      .setMaterial(transMat);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.5f, 1.f, 2.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-90.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(1.f));
    
    go = GameObject::Instantiate();

    go->AddComponent<MeshRenderer>()
      ->setMesh(quad)
      .setMaterial(material);
    
    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.f, -.5f, 0.f))
      .setLocalScale(glm::vec3(10.f, 1.f, 10.f));
    
    go = GameObject::Instantiate();

    go->AddComponent<Light>()
      ->setColour(glm::vec3(.99f, .72f, .5f))
      .setIntensity(10.f)
      .setDirectional()
      .setShadows(true);
    
    go->AddComponent<MeshRenderer>()
      ->setMesh(mesh);
    
    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.f, 3.f, 2.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-70.f, 30.f, 0.f))))
      .setLocalScale(glm::vec3(0.1f));

    
    /*
    go = GameObject::Instantiate();
    
    go->AddComponent<Light>()
      ->setColour(glm::vec3(0.f, 1.f, 0.f))
      .setIntensity(10.f)
      .setSpot(80.f, 90.f)
      .setPoint()
      .setRadius(25.f)
      .setShadows(true);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(1.f, 3.f, -1.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-90.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(0.1f));

    go->AddComponent<MeshRenderer>()
      ->setMesh(mesh);

    
    go = GameObject::Instantiate();

    go->AddComponent<Light>()
      ->setColour(glm::vec3(1.f, 1.f, 1.f))
      .setIntensity(50.f)
      .setSpot(1.f, 50.f)
      .setRadius(25.f)
      .setShadows(true);

    go->AddComponent<MeshRenderer>()
      ->setMesh(mesh);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.f, 5.f, 0.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-90.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(0.1f));*/
    
    int count = 10;
    for (int i = 0; i <= count; ++i)
    {
      go = GameObject::Instantiate();

      go->AddComponent<MeshRenderer>()
        ->setMesh(mesh)
        .setMaterial(material2);

      float angle = 360.f / count * i;
      float scale = 9;
      float x = glm::sin(angle) * scale;
      float y = glm::cos(angle) * scale;

      go->getComponent<Transform>()
        ->setPosition(glm::vec3(x, 0, y));
    }

    auto sponza = Load("resources/sponza/sponza.obj");
    sponza->getComponent<Transform>()->setLocalScale(glm::vec3(0.01f)).setRotation(glm::quat(glm::radians(glm::vec3(0.f, 90.f, 0.f))));
  }

} // engine