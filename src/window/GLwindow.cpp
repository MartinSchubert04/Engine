#include "GLwindow.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "window/GLwindow.h"
#include "window/Window.h"

using namespace window;

bool GLwindow::init(int width, int height, std::string title) {
  this->width = width;
  this->height = height;
  this->title = title;

  mRender->init(this);

  return mIsRunning;
}

GLwindow::~GLwindow() {
  // mUICtx->end();

  mRender->end();
}

bool GLwindow::isRunning() {
  return true;
}

void GLwindow::render() {
  // Clear the view
  mRender->preRender();

  // Initialize UI components
  // mUICtx->pre_render();

  // render scene to framebuffer and add it to scene view
  // mSceneView->render();

  // mPropertyPanel->render(mSceneView.get());

  // Render the UI
  // mUICtx->post_render();

  // Render end, swap buffers
  mRender->postRender();

  handleInput();
}

void GLwindow::onResize(int width, int height) {}

void GLwindow::onClose() {
  mIsRunning = false;
}

void GLwindow::handleInput() {

  if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
    mIsRunning = false;
}

void GLwindow::onKey(int key, int scancode, int action, int mods) {}
void GLwindow::onScroll(double delta) {}
