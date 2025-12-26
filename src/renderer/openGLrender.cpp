#include "openGLrender.h"
#include "Render.h"
#include "common.h"

namespace render {

static void on_key_callback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  auto pWindow =
      static_cast<window::Iwindow *>(glfwGetWindowUserPointer(window));
  pWindow->onKey(key, scancode, action, mods);
}

static void on_scroll_callback(GLFWwindow *window, double xoffset,
                               double yoffset) {
  auto pWindow =
      static_cast<window::Iwindow *>(glfwGetWindowUserPointer(window));
  pWindow->onScroll(yoffset);
}

static void on_window_size_callback(GLFWwindow *window, int width, int height) {
  auto pWindow =
      static_cast<window::Iwindow *>(glfwGetWindowUserPointer(window));
  pWindow->onResize(width, height);
}

static void on_window_close_callback(GLFWwindow *window) {
  window::Iwindow *pWindow =
      static_cast<window::Iwindow *>(glfwGetWindowUserPointer(window));
  pWindow->onClose();
}

bool OpenGLrenderer::init(window::Iwindow *window) {
  RenderContext::init(window);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  if (!glfwInit()) {
    fprintf(stderr, "Error: GLFW Window couldn't be initialized\n");
    return false;
  }
  glfwInit();

  auto glWindow = glfwCreateWindow(window->width, window->height,
                                   window->title.c_str(), nullptr, nullptr);

  window->setNativeWindow(glWindow);

  glfwSetWindowUserPointer(glWindow, window);
  glfwSetKeyCallback(glWindow, on_key_callback);
  glfwSetScrollCallback(glWindow, on_scroll_callback);
  glfwSetWindowSizeCallback(glWindow, on_window_size_callback);
  glfwSetWindowCloseCallback(glWindow, on_window_close_callback);
  glfwMakeContextCurrent(glWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  return true;
}

void OpenGLrenderer::preRender() {
  glViewport(0, 0, mWindow->width, mWindow->height);
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLrenderer::postRender() {
  glfwPollEvents();
  glfwSwapBuffers((GLFWwindow *)mWindow->getNativeWindow());
}

void OpenGLrenderer::end() {
  glfwDestroyWindow((GLFWwindow *)mWindow->getNativeWindow());
  glfwTerminate();
}

}  // namespace render
