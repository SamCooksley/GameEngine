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
    
    auto material = std::make_shared<graphics::Material>(*Graphics::getContext().defaultMaterial); //graphics::Material::Create(Resources::Load<graphics::Shader>("resources/shaders/default.shader"));

    material->setTexture("diffuse", graphics::Texture2D::Load("resources/textures/stone_wall/diffuse.jpg"));
    material->setTexture("normal", graphics::Texture2D::Load("resources/textures/stone_wall/normal.jpg"));
    material->setTexture("specular", graphics::Texture2D::Load("resources/textures/stone_wall/specular.jpg"));
    material->setTexture("displacement", graphics::Texture2D::Load("resources/textures/stone_wall/displacement.jpg"));
    material->setTexture("opacity", std::make_shared<graphics::Texture2D>(16, 16, glm::vec4(1.f)));
    material->setUniform("displacementScale", .02f);
    
    auto go = GameObject::Instantiate();

    
    auto mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    mr->setMaterial(material);

    go->getComponent<Transform>()->setPosition(glm::vec3(0, 0, -5));

    /*
    material = std::make_shared<graphics::Material>(*Graphics::getContext().defaultMaterial);

    material->setTexture("diffuse", graphics::Texture2D::Load("resources/textures/stone_wall_2/diffuse.jpg"));
    material->setTexture("normal", graphics::Texture2D::Load("resources/textures/stone_wall_2/normal.jpg"));
    material->setTexture("specular", graphics::Texture2D::Load("resources/textures/stone_wall_2/specular.jpg"));
    material->setTexture("displacement", graphics::Texture2D::Load("resources/textures/stone_wall_2/displacement.jpg"));
    material->setUniform("displacementScale", 0.2f);
    
    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    mr->setMaterial(material);
    

    go->getComponent<Transform>()->setPosition(glm::vec3(2, 0, -4));
    */
    auto transShader = Resources::Load<graphics::Shader>("resources/shaders/transparent.shader");
    auto transMat = std::make_shared<graphics::Material>(transShader);

    transMat->setTexture("diffuse", graphics::Texture2D::Load("resources/textures/window.png"));
    transMat->setTexture("normal", graphics::Texture2D::Load("resources/textures/flat.png"));
    transMat->setTexture("specular", std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(1.f)));
    transMat->setTexture("displacement", std::make_shared<graphics::Texture2D>(64, 64, glm::vec4(1.f)));

    auto quad = Resources::Load<graphics::Mesh>("resources/models/quad.obj");

    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);
    mr->setMaterial(transMat);

    go->getComponent<Transform>()->setPosition(glm::vec3(0.f, .5f, 1.f)).setRotation(glm::quat(glm::radians(glm::vec3(90.f, 0.f, 0.f)))).setLocalScale(glm::vec3(0.5f));

    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);
    mr->setMaterial(transMat);

    go->getComponent<Transform>()->setPosition(glm::vec3(-0.5f, 1.5f, 1.5f)).setRotation(glm::quat(glm::radians(glm::vec3(90.f, 0.f, 0.f)))).setLocalScale(glm::vec3(0.5f));

    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);
    mr->setMaterial(transMat);

    go->getComponent<Transform>()->setPosition(glm::vec3(0.5f, 1.f, 2.f)).setRotation(glm::quat(glm::radians(glm::vec3(90.f, 0.f, 0.f)))).setLocalScale(glm::vec3(0.5f));

#if 1
    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);
    
    go->getComponent<Transform>()->setPosition(glm::vec3(0.f, -.5f, 0.f)).setLocalScale(glm::vec3(10.f, 1.f, 10.f));

#else
    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);

    go->getComponent<Transform>()->setPosition(glm::vec3(2.5f, -.5f, 2.5f)).setLocalScale(glm::vec3(5.f, 1.f, 5.f));
   
    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);

    go->getComponent<Transform>()->setPosition(glm::vec3(2.5f, -.5f, -2.5f)).setLocalScale(glm::vec3(5.f, 1.f, 5.f));

    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);

    go->getComponent<Transform>()->setPosition(glm::vec3(-2.5f, -.5f, 2.5f)).setLocalScale(glm::vec3(5.f, 1.f, 5.f));

    go = GameObject::Instantiate();

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(quad);

    go->getComponent<Transform>()->setPosition(glm::vec3(-2.5f, -.5f, -2.5f)).setLocalScale(glm::vec3(5.f, 1.f, 5.f));
#endif
    
    go = GameObject::Instantiate();

    go->AddComponent<Light>()
      ->setColour(glm::vec3(.6f, .5f, .5f) * 0.5f)
      .setDirectional()
      .setShadows(true);
    
    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    
    go->getComponent<Transform>()
      ->setPosition(glm::vec3(0.f, 3.f, 2.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(-70.f, 30.f, 0.f))))
      .setLocalScale(glm::vec3(0.1f));
    
    go = GameObject::Instantiate();

    go->AddComponent<Light>()
      ->setColour(glm::vec3(1.f))
      .setSpot(80.f, 90.f)
      .setRadius(10.f)
      .setShadows(true);

    go->getComponent<Transform>()
      ->setPosition(glm::vec3(4.f, 2.f, -5.f))
      .setRotation(glm::quat(glm::radians(glm::vec3(0.f, 0.f, 0.f))))
      .setLocalScale(glm::vec3(0.1f));

    mr = go->AddComponent<MeshRenderer>();
    mr->setMesh(mesh);
    
    int count = 10;
    for (int i = 0; i <= count; ++i)
    {
      go = GameObject::Instantiate();

      mr = go->AddComponent<MeshRenderer>();
      mr->setMesh(mesh);
      mr->setMaterial(material);
      float angle = 360.f / count * i;
      float scale = 9;
      float x = glm::sin(angle) * scale;
      float y = glm::cos(angle) * scale;

      go->getComponent<Transform>()->setPosition(glm::vec3(x, 0, y));
    }

    auto sponza = Load("resources/sponza/sponza.obj");
    sponza->getComponent<Transform>()->setLocalScale(glm::vec3(0.01f)).setRotation(glm::quat(glm::radians(glm::vec3(0.f, 90.f, 0.f))));
  }

} // engine