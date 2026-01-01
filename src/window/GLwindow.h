#pragma once

#include "common.h"
#include "renderer/GLrenderer.h"
#include "renderer/UIcontext.h"
#include "elements/Camera.h"
#include "elements/Shader.h"
#include "elements/Model.h"
#include "ui/Scene.h"
#include "ui/Panel.h"

using namespace UI;

namespace window {

class GLwindow : public Iwindow {

public:
  GLwindow() : mIsRunning(true), mWindow(nullptr) {

    mUIcontext = std::make_unique<UIcontext>();
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

private:
  GLFWwindow *mWindow;
  bool mIsRunning;

  float mDeltaTime = 0.0f;
  float mLastFrame = 0.0f;
  float mPreviousTime;
  int mFrameCount = 0;
  float mFPS;

  std::unique_ptr<Model> mModel;
  std::unique_ptr<Shader> mShader;
  std::unique_ptr<GLrenderer> mRender;
  std::unique_ptr<UIcontext> mUIcontext;
  std::unique_ptr<Scene> mScene;
  std::unique_ptr<Panel> mPropertyPanel;

  Camera mCamera;

  void setTitle(std::string newTitle);
  void updateTitle();
  float getFPS();

  // std::unique_ptr<UIcontext> mSceneView;
};

}  // namespace window
