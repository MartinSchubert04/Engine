#include "Application.h"
#include "Assert.h"
#include "Core/Application.h"
#include "DeltaTime.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "GLFW/glfw3.h"
#include "pch.h"
#include "Core/Base.h"
#include "Core/Log.h"
#include "Renderer/Renderer.h"
#include "Debug/Instrumentor.h"

namespace Engine {

Application *Application::s_instance = nullptr;

Application::Application() {
  PROFILE_SCOPE("Application Init");

  CORE_ASSERT(!s_instance, "Error: Application instance already exists");
  s_instance = this;

  mWindow = Scope<Window>(Window::create());
  mWindow->setEventCallback(BIND_FN(Application ::onEvent));

  Renderer::init();

  mImGuiLayer = new ImGuiLayer;
  pushOverlay(mImGuiLayer);
}

Application::~Application() {}

void Application::run() {
  CORE_INFO("App running");

  CORE_INFO("OpenGL version: {0}", (const char *)glGetString(GL_VERSION));
  CORE_INFO("Vendor: {0}", (const char *)glGetString(GL_VENDOR));
  CORE_INFO("Renderer: {0}", (const char *)glGetString(GL_RENDERER));
  CORE_INFO("GLSL verson: {0}", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  CORE_INFO("GLFW version: {0}", glfwGetVersionString());

  while (mRunning) {
    PROFILE_SCOPE("Application Loop");

    float time = (float)glfwGetTime();
    DeltaTime dt = time - mLastFrameTime;
    mLastFrameTime = time;

    if (!mMinimized) {
      {
        PROFILE_SCOPE("LayerStack onUpdate");

        for (Layer *layer : mLayerStack) {
          layer->onUpdate(dt);
        }
      }
    }

    mImGuiLayer->begin();
    {
      PROFILE_SCOPE("LayerStack onImGuiRender");

      for (Layer *layer : mLayerStack)
        layer->onImGuiRender();
    }
    mImGuiLayer->end();

    mWindow->onUpdate();

    RenderCommand::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
    RenderCommand::clear();
  }
}

void Application::pushLayer(Layer *layer) {
  mLayerStack.pushLayer(layer);
  layer->onAttach();
}

void Application::pushOverlay(Layer *layer) {
  mLayerStack.pushOverlay(layer);
  layer->onAttach();
}

void Application::onEvent(Event &e) {

  // if (e.getEventType() != EventType::MouseMoved)
  //   CORE_TRACE("{0}", e.toString());

  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(BIND_FN(Application::onWindowClose));
  dispatcher.dispatch<WindowResizeEvent>(BIND_FN(Application::onWindowResize));

  for (auto it = mLayerStack.end(); it != mLayerStack.begin();) {
    (*--it)->onEvent(e);
    if (e.handled)
      break;
  }
}

bool Application::onWindowClose(WindowCloseEvent &e) {
  mRunning = false;
  return true;
}

bool Application::onWindowResize(WindowResizeEvent &e) {

  if (e.getHeight() == 0 || e.getWidth() == 0) {
    mMinimized = true;
    return false;
  }

  mMinimized = false;

  Renderer::onWindowResize(e.getWidth(), e.getHeight());
  return false;
}

}  // namespace Engine
