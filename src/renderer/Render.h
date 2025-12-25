#pragma once

#include "window/Window.h"

namespace render {

class VertexIndexBuffer {};

class FrameBuffer {};

class RenderContext {

public:
  RenderContext() : mWindow(nullptr) {}

  virtual bool init(window::Iwindow *window) {
    mWindow = window;
    return true;
  }

  virtual void preRender() = 0;
  virtual void postRender() = 0;
  virtual void end() = 0;

protected:
  window::Iwindow *mWindow;
};

}  // namespace render
