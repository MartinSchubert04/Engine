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

using namespace window;

bool GLwindow::init(int width, int height, std::string title) {
  this->width = width;
  this->height = height;
  this->title = title;

  mRender->init(this);

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
  mRender->preRender();

  // Initialize UI components
  mInterface->preRender();

  // render scene to framebuffer and add it to scene view
  // mSceneView->render();
  stbi_set_flip_vertically_on_load(true);

  Camera camera(glm::vec3(0.0, 0.0, 5.0));
  Model loader("resources/model/backpack.obj");
  Shader shader("shaders/model.vs", "shaders/model.fs");

  shader.use();

  shader.setFloat("time", glfwGetTime());
  shader.setVec3("viewPos", camera.Position);

  // spot light
  shader.setVec3("spotLight.position", camera.Position);
  shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  shader.setVec3("spotLight.direction", camera.Front);
  shader.setFloat("spotLight.constant", 1.0f);
  shader.setFloat("spotLight.linear", 0.09f);
  shader.setFloat("spotLight.quadratic", 0.032f);
  shader.setFloat("spotLight.cutOffAngle", glm::cos(glm::radians(12.5f)));
  shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

  glm::mat4 view =
      glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

  // create projection matrix
  glm::mat4 projection;
  projection =
      glm::perspective(glm::radians(camera.Zoom),
                       (float)this->width / (float)this->height, 0.1f, 100.0f);

  shader.setMat4("projection", projection);
  shader.setMat4("view", view);

  // render the loaded model
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(
      model,
      glm::vec3(0.0f, 0.0f,
                0.0f));  // translate it down so it's at the center of the scene
  model = glm::scale(
      model,
      glm::vec3(1.0f, 1.0f,
                1.0f));  // it's a bit too big for our scene, so scale it down
  shader.setMat4("model", model);

  loader.draw(shader);

  // mPropertyPanel->render(mSceneView.get());

  // Render the UI
  mInterface->postRender();

  // Render end, swap buffers
  mRender->postRender();

  handleInput();
}

void GLwindow::onResize(int width, int height) {}

void GLwindow::onClose() {
  mIsRunning = false;
}

void GLwindow::handleInput() {

  if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    mIsRunning = false;
}

void GLwindow::onKey(int key, int scancode, int action, int mods) {}
void GLwindow::onScroll(double delta) {}
