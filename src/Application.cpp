#include "Application.h"
#include <memory>

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Application::Application(const std::string &app_name) {
  //  mWindow = std::make_unique<window::GLwindow>();
  //  mWindow->init(1024, 720, app_name);
}

void Application::run() {
  while (mWindow->isRunning()) {
    mWindow->render();
  }
}
