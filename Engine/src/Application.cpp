#include "Application.h"
#include "pch.h"
#include "Core/Log.h"
namespace Core {

Application::Application(const std::string &app_name) {
  Engine::Log::init();

  mWindow = std::make_unique<window::GLwindow>();
  mWindow->init(1280, 720, app_name);
}

void Application::run() {
  CORE_INFO("RUNNING APP");

  while (mWindow->isRunning()) {
    mWindow->render();
  }
}

}  // namespace Core
