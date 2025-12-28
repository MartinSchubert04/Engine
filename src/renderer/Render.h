#pragma once

#include "window/Window.h"
#include "elements/Vertex.h"

#define ASSERT(x) \
  if (!(x)) \
    __builtin_trap();

#define DEBUG

#ifdef DEBUG
#define GLcall(x) \
  GLClearError(); \
  x; \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLcall(x) x
#endif

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

class Buffer {

public:
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual void destroy() = 0;

protected:
  GLuint mRendererID;
};

class FrameBuffer {};

}  // namespace render
