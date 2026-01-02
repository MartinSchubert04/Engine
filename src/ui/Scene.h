#pragma once

#include "elements/Camera.h"
#include "elements/Mesh.h"
// #include "elements/light.h"
#include "elements/Shader.h"
#include "elements/Light.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/FrameBuffer.h"
#include "elements/Input.h"
#include "elements/Model.h"

namespace UI {

class Scene {

public:
  Scene()
      : mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr),
        mLight(nullptr), mSize(800, 600) {

    mFrameBuffer = std::make_unique<FrameBuffer>();
    mFrameBuffer->create(800, 600);
    mShader = std::make_unique<Shader>("shaders/model.vs", "shaders/model.fs");
    mLight = std::make_unique<Light>();

    mCamera =
        std::make_unique<Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 100.0f);
  }

  ~Scene() { mShader->unload(); }

  // nelems::Light *get_light() { return mLight.get(); }

  void resize(int32_t width, int32_t height);

  void render();

  void loadModel(const std::string &filepath);

  void setModel(std::shared_ptr<Model> model) { mModel = model; }

  std::shared_ptr<Model> getModel() { return mModel; }

  Light *getLight() { return mLight.get(); }

  std::shared_ptr<Camera> getCamera() { return mCamera; }

  void onMouseMove(double x, double y, InputType button);

  void onMouseWheel(double delta);

  void resetView() { mCamera->reset(); }

private:
  std::shared_ptr<Camera> mCamera;
  std::unique_ptr<FrameBuffer> mFrameBuffer;
  std::unique_ptr<Shader> mShader;
  std::unique_ptr<Light> mLight;
  std::shared_ptr<Model> mModel;
  glm::vec2 mSize;
};

}  // namespace UI
