#include "Application.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "pch.h"

namespace Engine {

Application::Application() {
  CORE_INFO("Previo intancia");
  mWindow = Scope<Window>(Window::create());
}

Application::~Application() {}

void Application::run() {

  while (mRunning) {
    glClearColor(.8, .8, .8, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    mWindow->onUpdate();
  }
}

}  // namespace Engine
