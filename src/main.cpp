#include "Application.h"
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "elements/Shader.h"
#include "elements/Camera.h"
#include "elements/Model.h"

#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
std::string updateFrameRate(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
unsigned int loadTexture(char const *path);

static const float SCR_WIDTH = 800.f;
static const float SCR_HEIGHT = 800.f;

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of last frame
float previousTime = glfwGetTime();
int frameCount = 0;

float lastX = float(SCR_WIDTH) / 2, lastY = float(SCR_HEIGHT) / 2;
bool uiMode = false;
bool firstMouse = true;

Camera camera(glm::vec3(0.0, 0.0, 5.0));

int main() {
  auto app = std::make_unique<Application>("Model loader");

  app->run();

  // glfwInit();
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // // glfw window creation
  // // --------------------
  // GLFWwindow *window =
  //     glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  // if (window == NULL) {
  //   std::cout << "Failed to create GLFW window" << std::endl;
  //   glfwTerminate();
  //   return -1;
  // }

  // glfwMakeContextCurrent(window);
  // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  // //  : glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // // mouse input callbacks
  // glfwSetCursorPosCallback(window, mouse_callback);
  // // glfwSetScrollCallback(window, scroll_callback);

  // // glad: load all OpenGL function pointers
  // // ---------------------------------------
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //   std::cout << "Failed to initialize GLAD" << std::endl;
  //   return -1;
  // }

  // // configure global opengl state
  // // -----------------------------
  // glEnable(GL_DEPTH_TEST);

  // stbi_set_flip_vertically_on_load(true);

  // Shader shader("shaders/model.vs", "shaders/model.fs");

  // Model loader("resources/model/backpack.obj");

  // unsigned int diffuseMap = loadTexture("resources/textures/container2.png");
  // unsigned int specularMap =
  //     loadTexture("resources/textures/container2_specular.png");

  // unsigned int emisionMap = loadTexture("resources/textures/matrix.jpg");

  // shader.use();

  // while (!glfwWindowShouldClose(window)) {
  //   glClearColor(0.2, 0.2, 0.2, 1.0);
  //   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //   processInput(window);
  //   updateFrameRate(window);

  //   shader.use();
  //   shader.setFloat("time", glfwGetTime());
  //   shader.setVec3("viewPos", camera.Position);

  //   // spot light
  //   shader.setVec3("spotLight.position", camera.Position);
  //   shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  //   shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  //   shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  //   shader.setVec3("spotLight.direction", camera.Front);
  //   shader.setFloat("spotLight.constant", 1.0f);
  //   shader.setFloat("spotLight.linear", 0.09f);
  //   shader.setFloat("spotLight.quadratic", 0.032f);
  //   shader.setFloat("spotLight.cutOffAngle", glm::cos(glm::radians(12.5f)));
  //   shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

  //   glm::mat4 view =
  //       glm::lookAt(camera.Position, camera.Position + camera.Front,
  //       camera.Up);

  //   // create projection matrix
  //   glm::mat4 projection;
  //   projection =
  //       glm::perspective(glm::radians(camera.Zoom),
  //                        (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

  //   shader.setMat4("projection", projection);
  //   shader.setMat4("view", view);

  //   // render the loaded model
  //   glm::mat4 model = glm::mat4(1.0f);
  //   model = glm::translate(
  //       model,
  //       glm::vec3(
  //           0.0f, 0.0f,
  //           0.0f));  // translate it down so it's at the center of the scene
  //   model = glm::scale(
  //       model,
  //       glm::vec3(1.0f, 1.0f,
  //                 1.0f));  // it's a bit too big for our scene, so scale it
  //                 down
  //   shader.setMat4("model", model);

  //   loader.draw(shader);

  //   glfwSwapBuffers(window);
  //   glfwPollEvents();
  // }

  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  static bool rightMousePressedLastFrame = false;

  bool rightMousePressedNow =
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

  if (rightMousePressedNow && !rightMousePressedLastFrame) {
    uiMode = !uiMode;

    if (uiMode) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      firstMouse = true;
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      firstMouse = true;
    }
  }

  rightMousePressedLastFrame = rightMousePressedNow;
  if (uiMode)
    return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  if (uiMode)
    return;

  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos;  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

std::string updateFrameRate(GLFWwindow *window) {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  frameCount++;

  // Actualizar cada 0.25 segundos
  if (currentFrame - previousTime >= 0.25) {
    float fps = frameCount / (currentFrame - previousTime);

    std::string fpsCountString = std::to_string((int)fps);

    previousTime = currentFrame;
    frameCount = 0;

    return fpsCountString;
  }
  return "";
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
