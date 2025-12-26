#pragma once

#include "window/Window.h"
#include "elements/Vertex.h"

namespace render {

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

class VertexIndexBuffer {

public:
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void destroy() = 0;
};

class FrameBuffer {};

}  // namespace render
