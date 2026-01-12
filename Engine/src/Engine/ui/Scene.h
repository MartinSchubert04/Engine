#pragma once

#include "Core/Camera.h"
#include "Core/Mesh.h"
// #include "Core/light.h"
#include "Core/Shader.h"
#include "Core/Light.h"
#include "Renderer/VertexBuffer.h "
#include "Renderer/IndexBuffer.h "
#include "Renderer/FrameBuffer.h "
#include "Core/Input.h"
#include "Core/Model.h"
#include "Sim/src/System.h"
#include "Sim/src/Grid.h"

namespace UI {

class Scene {

public:
  float ballSize = 0.5;
  int ballCount = 200;

  Scene() : mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr), mLight(nullptr), mSize(800, 600) {

    mFrameBuffer = std::make_unique<FrameBuffer>();
    mFrameBuffer->create(800, 600);
    mShader = std::make_unique<Shader>("shaders/model.vs", "shaders/model.fs");
    mLight = std::make_unique<Light>();

    mCamera = std::make_unique<Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 1000.0f);

    mCamera->setDistance(20);
    mContainer = std::make_unique<Planet>(10, glm::vec2(10, 10), glm::vec3(0, 0, 0));

    mSystem = std::make_unique<universe::System>();

    std::vector<glm::vec3> positions = mContainer->getVerticesPos();

    for (auto &p : positions) {
      std::unique_ptr<Planet> sphere = std::make_unique<Planet>(0.1, glm::vec2(15, 15), p);
      mContainerBoudaries.push_back(std::move(sphere));
    }
  }

  ~Scene() { mShader->unload(); }

  // nelems::Light *get_light() { return mLight.get(); }

  void resize(int32_t width, int32_t height);

  void render(float delta);

  void loadModel(const std::string &filepath);

  void setModel(std::shared_ptr<Model> model) { mModel = model; }

  std::shared_ptr<Model> getModel() { return mModel; }

  Light *getLight() { return mLight.get(); }

  std::shared_ptr<Camera> getCamera() { return mCamera; }

  void onMouseMove(double x, double y, InputType button);

  void onMouseWheel(double delta);

  void resetView() { mCamera->reset(); }

  inline void clearSystem() { mSystem->planets.clear(); }
  inline void setSphereSize() { mSystem->setSize(ballSize); }
  void setSphereCount() {
    for (int i{0}; i < ballCount; i++) {
      glm::vec3 randomPos = {(rand() % 100) / 500.0f, (rand() % 100) / 500.0f, (rand() % 100) / 500.0f};
      std::unique_ptr<Planet> sphere = std::make_unique<Planet>(ballSize, glm::vec2(25, 25), randomPos, 1);
      mSystem->addEntity(std::move(sphere));
    }
  }

private:
  std::shared_ptr<Camera> mCamera;
  std::unique_ptr<FrameBuffer> mFrameBuffer;
  std::unique_ptr<Shader> mShader;
  std::unique_ptr<Light> mLight;
  std::shared_ptr<Model> mModel;
  std::shared_ptr<universe::System> mSystem;
  std::shared_ptr<Planet> mContainer;

  Grid grid = Grid(glm::vec2(200, 200), glm::vec3(.0f, -5.f, .0f));

  std::vector<std::unique_ptr<Planet>> mContainerBoudaries;

  glm::vec2 mSize;
};

}  // namespace UI
