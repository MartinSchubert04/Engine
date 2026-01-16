#include "Application.h"
#include "Assert.h"
#include "Core/Application.h"
#include "pch.h"
#include "Core/Base.h"
#include "Core/Log.h"

namespace Engine {

Application *Application::s_instance = nullptr;

Application::Application() {

  CORE_ASSERT(!s_instance, "Error: Application instance already exists");
  s_instance = this;

  mWindow = Scope<Window>(Window::create());
  mWindow->setEventCallback(BIND_FN(Application ::onEvent));
}

Application::~Application() {}

void Application::run() {
  CORE_INFO("Prerun");

  while (mRunning) {
    glClearColor(.2, .2, .2, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    mWindow->onUpdate();

    CORE_TRACE("LayerStack OnUpdate");

    for (Layer *layer : mLayerStack)
      layer->onUpdate();
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

  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowCloseEvent>(BIND_FN(Application::onWindowClose));
  dispatcher.dispatch<WindowResizeEvent>(BIND_FN(Application::onWindowResize));

  CORE_TRACE("{0}", e.toString());
}

bool Application::onWindowClose(WindowCloseEvent &e) {
  mRunning = false;
  return true;
}

bool Application::onWindowResize(WindowResizeEvent &e) {

  return true;
}

}  // namespace Engine
