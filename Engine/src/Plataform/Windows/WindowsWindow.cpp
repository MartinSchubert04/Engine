#include "WindowsWindow.h"
#include "Core/Assert.h"
#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "pch.h"
#include "Core/Log.h"
#include "Core/Base.h"

namespace Engine {

static uint8_t s_GLFWWindowCOunt = 0;

static void GLFWErrorCallback(int error, const char *description) {
  CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

WindowsWindow::WindowsWindow(const WindowProps &props) {
  init(props);
}

WindowsWindow::~WindowsWindow() {
  shutdown();
}

void WindowsWindow::init(const WindowProps &props) {

  if (s_GLFWWindowCOunt == 0) {
    int success = glfwInit();
    CORE_ASSERT(success, "Could not initialize GLFW");
    glfwSetErrorCallback(GLFWErrorCallback);
  }

  {
    // create window scope

    // #if defined(DEBUG)
    //   if (Renderer::getA)
    // #endif

    mWindow = glfwCreateWindow(props._width, props._height, props._title.c_str(), nullptr, nullptr);
    ++s_GLFWWindowCOunt;
  }

  glfwMakeContextCurrent(mWindow);
  glfwSetWindowUserPointer(mWindow, &mData);
  setVSync(true);
}

void WindowsWindow::shutdown() {
  glfwDestroyWindow(mWindow);
}

void WindowsWindow::onUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(mWindow);
}

void WindowsWindow::setVSync(const bool enable) {
  if (enable) {
    glfwSwapInterval(1);
  } else
    glfwSwapInterval(0);

  mData.vsync = enable;
}

}  // namespace Engine
