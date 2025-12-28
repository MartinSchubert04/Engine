#pragma once

#include "elements/Camera.h"
#include "elements/Mesh.h"
// #include "elements/light.h"
#include "elements/Shader.h"
#include "renderer/VertexBuffer.h"
#include "renderer/IndexBuffer.h"
#include "elements/Input.h"

namespace nui {
class SceneView {
public:
  SceneView()
      : mCamera(nullptr), mFrameBuffer(nullptr), mShader(nullptr),
        mLight(nullptr), mSize(800, 600) {
    mFrameBuffer = std::make_unique<FrameBuffer>();
    mFrameBuffer->create_buffers(800, 600);
    mShader = std::make_unique<Shader>();
    mShader->load("shaders/vs.shader", "shaders/fs_pbr.shader");
    // mLight = std::make_unique<nelems::Light>();

    mCamera =
        std::make_unique<Camera>(glm::vec3(0, 0, 3), 45.0f, 1.3f, 0.1f, 100.0f);
  }

  ~SceneView() { mShader->unload(); }

  // nelems::Light *get_light() { return mLight.get(); }

  void resize(int32_t width, int32_t height);

  void render();

  void loadMesh(const std::string &filepath);

  void setMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }

  std::shared_ptr<Mesh> get_mesh() { return mMesh; }

  void onMouseMove(double x, double y, InputType button);

  void onMouseWheel(double delta);

  void resetView() { mCamera->reset(); }

private:
  std::unique_ptr<Camera> mCamera;
  std::unique_ptr<FrameBuffer> mFrameBuffer;
  std::unique_ptr<Shader> mShader;
  // std::unique_ptr<nelems::Light> mLight;
  std::shared_ptr<Mesh> mMesh;
  glm::vec2 mSize;
};
}  // namespace nui
