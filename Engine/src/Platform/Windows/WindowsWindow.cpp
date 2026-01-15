#include "WindowsWindow.h"
#include "Core/Assert.h"
#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "pch.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

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
    CORE_ASSERT(mWindow, "Failed to create window context");
  }

  glfwMakeContextCurrent(mWindow);
  glfwSetWindowUserPointer(mWindow, &mData);
  glfwSetErrorCallback(GLFWErrorCallback);

  {
    int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    CORE_ASSERT(success, "Failed to load GLAD context");
  }
  setVSync(true);

  glfwSetWindowSizeCallback(mWindow, [](GLFWwindow *window, int width, int height) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    data.height = height;
    data.width = width;

    WindowResizeEvent event(width, height);
    data.EventCallback(event);
  });
  glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *window) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;

    data.EventCallback(event);
  });
  glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int scanscode, int action, int mods) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    switch (action) {
    case GLFW_PRESS: {
      KeyPressedEvent event(key, false);
      data.EventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent event(key);
      data.EventCallback(event);
      break;
    }
    case GLFW_REPEAT: {
      KeyPressedEvent event(key, true);
      data.EventCallback(event);
      break;
    }
    }
  });

  glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    switch (action) {

    case GLFW_PRESS: {
      MouseButtonPressedEvent event(button);
      data.EventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      MouseButtonReleasedEvent event(button);
      data.EventCallback(event);
      break;
    }
    }
  });

  glfwSetScrollCallback(mWindow, [](GLFWwindow *window, double yOffset, double xOffset) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseScrolledEvent event((float)xOffset, (float)yOffset);
    data.EventCallback(event);
  });

  glfwSetCursorPosCallback(mWindow, [](GLFWwindow *window, double xpos, double ypos) {
    WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);

    MouseMovedEvent event((float)xpos, (float)ypos);
    data.EventCallback(event);
  });
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
