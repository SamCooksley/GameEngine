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

    s_context->glfwContext.reset(new glfw());

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

    debug::Log("OpenGL version: " + String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    debug::Log("GLSL version: " + String(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
 
    s_context->graphics = std::make_unique<graphics::Context>();

    //uniform buffers
    {
      auto camera = std::make_unique<graphics::CameraBuffer>();
      s_context->graphics->uniformBuffers.Add(std::move(camera));

      auto lights = std::make_unique<graphics::LightBuffer>();
      s_context->graphics->uniformBuffers.Add(std::move(lights));
    }

    s_context->graphics->defaultFrameBuffer = graphics::FrameBuffer::CreateDefault(640, 480);
    s_context->graphics->screenQuad = graphics::mesh::Quad().getMesh();
    
    s_context->graphics->errorShader = Resources::Load<graphics::Shader>("resources/shaders/error.shader");

    auto defaultShader = Resources::Load<graphics::Shader>("resources/shaders/gbuffer.shader");
    s_context->graphics->defaultMaterial = graphics::Material::Create(defaultShader);
   
    auto texture = graphics::Texture2D::Create(1, 1, glm::vec4(0.9f));
    s_context->graphics->defaultMaterial->setTexture("diffuse", texture);

    texture = graphics::Texture2D::Create(1, 1, glm::vec4(.5f, .5f, 1.f, 1.f));
    s_context->graphics->defaultMaterial->setTexture("normal", texture);

    texture = graphics::Texture2D::Create(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("specular", texture);
    texture = graphics::Texture2D::Create(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("displacement", texture);

    texture = graphics::Texture2D::Create(1, 1, glm::vec4(1.f));
    s_context->graphics->defaultMaterial->setTexture("opacity", texture);

    s_context->graphics->defaultMaterial->setUniform("displacementScale", 0.01f);
        
    s_context->graphics->defaultMaterial->setUniform("shininess", 5.f);
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
    for (auto & l : s_context->shadowLights)
    {
      auto light = l.lock();
      assert(light);

      if (!light->getEnabled()) { continue; }

      light->SetupShadowPass();
      auto shadow = light->getShadow();
      auto renderer = shadow->getRenderer();

      //TODO: render to shadowbuffer
      if (s_context->scene)
      {
        s_context->scene->Render(*renderer);
      }

      renderer->End();

      renderer->Render();
    }

    graphics::FrameBuffer::BindDefault();
    s_context->graphics->defaultFrameBuffer->Clear();

    if (s_context->cameras.empty())
    {
      debug::LogError("Camera Error: no cameras in scene");
    }

    for (auto & cam : s_context->cameras)
    {
      auto camera = cam.lock();
      assert(camera);

      if (!camera->getEnabled()) { continue; }

      camera->SetupRender();

      camera->m_renderer->Start(camera->getCamera());

      if (s_context->scene)
      {
        s_context->scene->Render(*camera->m_renderer);
      }

      camera->m_renderer->End();

      camera->m_renderer->Render();
    }

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