#pragma once

#include "common.h"
#include "renderer/GLrenderer.h"
#include "renderer/Interface.h"
#include "elements/Camera.h"
#include "elements/Shader.h"
#include "elements/Model.h"

// #include "renderer/ui_context.h"
// #include "renderer/opengl_context.h"
// #include "renderer/opengl_buffer_manager.h"

namespace window {

class GLwindow : public Iwindow {

public:
  GLwindow() : mIsRunning(true), mWindow(nullptr) {

    mInterface = std::make_unique<Interface>();
    mRender = std::make_unique<GLrenderer>();
  }

  ~GLwindow();

  bool init(int width, int height, std::string title);
  bool isRunning();
  void render();

  void setNativeWindow(void *window) override {
    mWindow = (GLFWwindow *)window;
  }

  void *getNativeWindow() override { return mWindow; }

  void onScroll(double delta) override;

  void onKey(int key, int scancode, int action, int mods) override;

  void onResize(int width, int height) override;

  void onClose() override;

  bool inRunning() { return mIsRunning; }

  void handleInput();

  void setTitle(std::string newTitle);
  void updateFrameRate();

private:
  GLFWwindow *mWindow;
  bool mIsRunning;

  std::unique_ptr<Model> mModel;
  std::unique_ptr<Shader> mShader;

  std::unique_ptr<GLrenderer> mRender;

  std::unique_ptr<Interface> mInterface;

  Camera mCamera;

  // std::unique_ptr<Interface> mSceneView;
};

}  // namespace window
