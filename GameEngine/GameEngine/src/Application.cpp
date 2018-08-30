#include "stdafx.h"  

#include "Application.h"

#include "graphics\opengl.h"

#include "Input.h"
#include "graphics\Shader.h"

#include "graphics\DefaultRenderer.h"

#include "graphics\Texture2D.h"
#include "graphics\TextureCube.h"

#include "Resources.h"

#include "graphics\CameraBuffer.h"
#include "graphics\LightBuffer.h"

#include "utilities\System.h"

#include "graphics\mesh\quad.h"

#include "glfw\glfw3.h"

#include "Random.h"

namespace engine {

  std::unique_ptr<Context> Application::s_context;

  void Application::Init(int _argc, char** _argv)
  {
    s_context.reset(new Context());

    s_context->state = EngineState::Init;

    for (int i = 0; i < _argc; ++i)
    {
      s_context->args.push_back(_argv[i]);
    }

    Random::Init();

    s_context->glfwContext.reset(new glfw());

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    s_context->window.reset(new graphics::Window("Engine", 640, 480));
    s_context->window->setVsync(true);

    s_context->targetFrameTime = 1.f / 60.f;
    s_context->maxUpdatesPerFrame = 5;
    s_context->totalDeltaTime = 0.f;
    s_context->deltaTime = 0.f;

    Input::Init();

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
      throw std::runtime_error("glew error: " + std::to_string(error) + ": " + String((const char*)glewGetErrorString(error)));
    }

    s_context->graphics = std::make_unique<graphics::Context>();
    s_context->graphics->debug = std::make_unique<graphics::DebugOutput>();
    s_context->graphics->debug->AddFilter(false, graphics::DebugSource::API, graphics::DebugType::OTHER, graphics::DebugSeverity::ANY, { 131185u });

    s_context->graphics->defaultFrameBuffer = graphics::FrameBufferDefault::Create(s_context->window->getWidth(), s_context->window->getHeight());
    s_context->graphics->captureFBO = graphics::FrameBuffer::Create(0, 0, 0);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    //uniform buffers
    {
      auto camera = std::make_unique<graphics::CameraBuffer>();
      s_context->graphics->uniformBuffers.Add(std::move(camera));

      auto lights = std::make_unique<graphics::LightBuffer>();
      s_context->graphics->uniformBuffers.Add(std::move(lights));
    }
    
    s_context->graphics->quad = graphics::mesh::Quad().getMesh();
    s_context->graphics->invCube = graphics::mesh::Skybox().getMesh();
    
    s_context->graphics->errorShader = Resources::Load<graphics::Shader>("resources/shaders/error.glsl");

    //s_context->graphics->depthShader = Resources::Load<graphics::Shader>("resources/shaders/depth.glsl");

    auto defaultShader = Resources::Load<graphics::Shader>("resources/shaders/pbr/deferred/gbuffer.glsl");
    s_context->graphics->defaultMaterial = std::make_shared<graphics::Material>(defaultShader);
   
    auto texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(0.9f));
    s_context->graphics->defaultMaterial->setTexture("albedo", texture);

    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(.5f, .5f, 1.f, 1.f));
    s_context->graphics->defaultMaterial->setTexture("normal", texture);

    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(0.f));
    s_context->graphics->defaultMaterial->setTexture("metallic", texture);
    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(.5f));
    s_context->graphics->defaultMaterial->setTexture("roughness", texture);

    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("opacity", texture);

    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("height", texture);
    s_context->graphics->defaultMaterial->setUniform("heightScale", 0.0f);

    texture = std::make_shared<graphics::Texture2D>(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("ao", texture);

    s_context->renderer = std::make_unique<graphics::DefaultRenderer>();

    {
      auto skyboxShader = Resources::Load<graphics::Shader>("resources/shaders/skybox.glsl");
      auto skyboxMaterial = std::make_shared<graphics::Material>(skyboxShader);

      std::array<String, 6u> skyboxTexturePaths;
      for (size_t i = 0u; i < 6u; ++i)
      {
        skyboxTexturePaths[i] = "resources/textures/skybox/skybox" + std::to_string(i + 1) + ".jpg";
      }

      auto skyboxCube = graphics::TextureCube::Load(skyboxTexturePaths);
      skyboxMaterial->setTexture("cubemap", skyboxCube);

      auto skybox = std::make_shared<graphics::Skybox>(skyboxMaterial);
      s_context->renderer->setSkybox(skybox);
    }
  }

  void Application::Loop()
  {
    s_context->state = EngineState::Running;
    s_context->frameTime.Reset();

    while (s_context->state == EngineState::Running)
    {
      Frame();
    }
  }

  void Application::Exit()
  {
    //destroy scene
    s_context->nextScene = nullptr;
    ChangeScene();

    Resources::Clear();

    s_context->renderer = nullptr;

    Destroy(s_context->graphics);
    Destroy(s_context);
  }

  void Application::Quit()
  {
    s_context->state = EngineState::Exit;
  }

  void Application::Frame()
  {
    if (s_context->targetFrameTime > 0.0f && s_context->deltaTime < s_context->targetFrameTime)
    {
      //utilities::Sleep(uint((s_context->targetFrameTime - s_context->deltaTime) * 1000.0f));
    }
    //s_context->totalDeltaTime += s_context->frameTime.getSeconds();
    s_context->deltaTime = s_context->frameTime.getSeconds();
    s_context->frameTime.Reset();

    if (s_context->nextScene)
    {
      ChangeScene();
    }

    //s_context->deltaTime = s_context->targetFrameTime;
    
    /*int count = 0;
    while (s_context->totalDeltaTime >= s_context->targetFrameTime)
    {
      s_context->deltaTime = s_context->targetFrameTime;
      HandleEvents();
      Update();
      s_context->totalDeltaTime -= s_context->targetFrameTime;

      if (++count > s_context->maxUpdatesPerFrame) { break; }
    }*/

    HandleEvents();
    Update();
    Render();
  }

  void Application::HandleEvents()
  {
    Input::Reset();
    s_context->glfwContext->PollEvents();
  }

  void Application::Update()
  {
    if (s_context->scene)
    {
      s_context->scene->Update();
    }
  }

  void Application::Render()
  {
    assert(s_context->camera && "No camera in scene");
    Camera & camera = *s_context->camera;

    assert(s_context->renderer && "No renderer");
    graphics::BaseRenderer & renderer = *s_context->renderer;
    renderer.Reset();

    auto & target = camera.getRenderTarget();

    int width = target.getWidth();
    int height = target.getHeight();
    renderer.Resize(width, height);

    float aspect = static_cast<float>(width) / static_cast<float>(height);
    auto cam = camera.getCamera(aspect);

    if (s_context->scene)
    {
      s_context->scene->Render(renderer);
    }

    target.Bind();
    target.Clear();

    
    renderer.setCamera(cam);

    renderer.End();

    renderer.Render();
    
    s_context->window->Present();
  }

  void Application::ChangeScene()
  {
    s_context->scene = std::move(s_context->nextScene);
    s_context->nextScene = nullptr;

    if (s_context->scene)
    {
      s_context->scene->Init();
    }
  }

} // engine