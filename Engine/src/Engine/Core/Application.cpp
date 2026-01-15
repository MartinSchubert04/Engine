#include "Application.h"
#include "Core/Application.h"
#include "pch.h"
#include "Core/Base.h"
#include "Core/Log.h"

namespace Engine {

Application::Application() {
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
  }
}

void Application::onEvent(Event &e) {
  CORE_INFO("{0}", e.toString());
}

}  // namespace Engine
