#include "GLwindow.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "window/GLwindow.h"

#include "elements/Camera.h"
#include "elements/Model.h"
#include "utils/TextureUtil.h"
#include "utils/Timer.h"

using namespace window;

bool GLwindow::init(int width, int height, std::string title) {
  this->width = width;
  this->height = height;
  this->title = title;

  mCamera.Position = glm::vec3(0.0, 0.0, 5.0);

  mRender->init(this);

  mPreviousTime = glfwGetTime();

  mScene = std::make_unique<Scene>();

  mPropertyPanel = std::make_unique<Panel>();

  mPropertyPanel->setModel_load_callback(
      [this](std::string filepath) { mScene->loadModel(filepath); });

  mUIcontext->init(this);

  return mIsRunning;
}

GLwindow::~GLwindow() {
  mUIcontext->end();
  mRender->end();
}

bool GLwindow::isRunning() {
  return mIsRunning;
}

void GLwindow::render() {
  // ScopedTimer t("Window render time");

  // Clear the view
  mRender->preRender();

  // Initialize UI components
  mUIcontext->preRender();

  updateTitle();

  // render scene to framebuffer and add it to scene view
  mScene->render();

  mPropertyPanel->render(mScene.get());

  // Render the UI
  mUIcontext->postRender();

  // Render end, swap buffers
  mRender->postRender();

  handleInput();
}

void GLwindow::onResize(int width, int height) {
  this->width = width;
  this->height = height;

  mScene->resize(this->width, this->height);
  render();
}

void GLwindow::onClose() {
  mIsRunning = false;
}

void GLwindow::handleInput() {

  float speed = 5.f;

  if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS) {
    mScene->onMouseWheel(-speed * mDeltaTime);
  }

  if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS) {
    mScene->onMouseWheel(speed * mDeltaTime);
  }

  if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS) {
    mScene->resetView();
  }

  double x, y;
  glfwGetCursorPos(mWindow, &x, &y);

  mScene->onMouseMove(x, y, Input::getInputPressed(mWindow));
}

void GLwindow::onKey(int key, int scancode, int action, int mods) {}
void GLwindow::onScroll(double delta) {
  mScene->onMouseWheel(delta);
}

void GLwindow::setTitle(std::string newTitle) {}

void GLwindow::updateTitle() {

  glfwSetWindowTitle(mWindow,
                     std::format("{} | FPS: {:.2f}", title, getFPS()).c_str());
}

float GLwindow::getFPS() {
  float currentFrame = glfwGetTime();
  mDeltaTime = currentFrame - mLastFrame;
  mLastFrame = currentFrame;
  mFrameCount++;

  if (currentFrame - mPreviousTime >= 1) {
    mFPS = mFrameCount / (currentFrame - mPreviousTime);

    mPreviousTime = currentFrame;
    mFrameCount = 0;
  }

  return std::round(mFPS * 100.0) / 100.0;
}
