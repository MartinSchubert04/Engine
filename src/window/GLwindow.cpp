#include "GLwindow.h"
#include "GLFW/glfw3.h"
#include "common.h"
#include "window/GLwindow.h"
#include "window/Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

  stbi_set_flip_vertically_on_load(true);
  mModel = std::make_unique<Model>("resources/model/backpack.obj");
  mShader = std::make_unique<Shader>("shaders/model.vs", "shaders/model.fs");

  mInterface->init(this);

  return mIsRunning;
}

GLwindow::~GLwindow() {
  mInterface->end();
  mRender->end();
}

bool GLwindow::isRunning() {
  return mIsRunning;
}

void GLwindow::render() {
  // Clear the view
  {
    // ScopedTimer t("PreRender Render");
    mRender->preRender();
  }

  // Initialize UI components
  {
    // ScopedTimer t("PreRender Interface");
    mInterface->preRender();
  }
  updateFrameRate();
  // render scene to framebuffer and add it to scene view
  // mSceneView->render();
  {
    // ScopedTimer t("Draw");
    GLuint query;
    glGenQueries(1, &query);

    {
    }

    mShader->use();

    mShader->setFloat("time", glfwGetTime());
    mShader->setVec3("viewPos", mCamera.Position);

    // spot light
    mShader->setVec3("spotLight.position", mCamera.Position);
    mShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    mShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    mShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    mShader->setVec3("spotLight.direction", mCamera.Front);
    mShader->setFloat("spotLight.constant", 1.0f);
    mShader->setFloat("spotLight.linear", 0.09f);
    mShader->setFloat("spotLight.quadratic", 0.032f);
    mShader->setFloat("spotLight.cutOffAngle", glm::cos(glm::radians(12.5f)));
    mShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    glm::mat4 view = glm::lookAt(mCamera.Position,
                                 mCamera.Position + mCamera.Front, mCamera.Up);

    // create projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(mCamera.Zoom),
                                  (float)this->width / (float)this->height,
                                  0.1f, 100.0f);

    mShader->setMat4("projection", projection);
    mShader->setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(
        model,
        glm::vec3(
            0.0f, 0.0f,
            0.0f));  // translate it down so it's at the center of the scene
    model = glm::scale(
        model,
        glm::vec3(1.0f, 1.0f,
                  1.0f));  // it's a bit too big for our scene, so scale it down
    mShader->setMat4("model", model);

    {
      // ScopedTimer t("Draw model");
      glBeginQuery(GL_TIME_ELAPSED, query);
      mModel->draw(*mShader);
      glEndQuery(GL_TIME_ELAPSED);

      GLuint64 time;
      glGetQueryObjectui64v(query, GL_QUERY_RESULT, &time);
      double gpuMs = time / 1e6;

      // std::cout << "[GPU MODEL DRAW TIME]" << gpuMs << std::endl;
    }
  }

  // mPropertyPanel->render(mSceneView.get());

  // Render the UI
  mInterface->postRender();

  // Render end, swap buffers
  mRender->postRender();

  handleInput();
}

void GLwindow::onResize(int width, int height) {
  this->width = width;
  this->height = height;

  // sceneView.onResize(this->width, this->height)

  render();
}

void GLwindow::onClose() {
  mIsRunning = false;
}

void GLwindow::handleInput() {

  float deltaTime = 0.004f;

  if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    mIsRunning = false;

  if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
    mCamera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
    mCamera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
    mCamera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
    mCamera.ProcessKeyboard(RIGHT, deltaTime);

  if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    mCamera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    mCamera.ProcessKeyboard(DOWN, deltaTime);

  double x, y;
  glfwGetCursorPos(mWindow, &x, &y);
}

void GLwindow::onKey(int key, int scancode, int action, int mods) {}
void GLwindow::onScroll(double delta) {}

void GLwindow::setTitle(std::string newTitle) {}

void GLwindow::updateFrameRate() {
  static float lastTime = 0.0f;
  static int frames = 0;

  float currentTime = glfwGetTime();
  frames++;

  if (currentTime - lastTime >= 1.f) {
    float fps = frames / (currentTime - lastTime);

    glfwSetWindowTitle(
        mWindow, (this->title + " | FPS: " + std::to_string((int)fps)).c_str());

    frames = 0;
    lastTime = currentTime;
  }
}
