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

  virtual void preRender();
  virtual void postRender();
  virtual void end();

protected:
  window::Iwindow *mWindow;
};

}  // namespace render
