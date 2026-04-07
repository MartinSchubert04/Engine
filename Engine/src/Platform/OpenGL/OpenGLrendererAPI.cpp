#include "OpenGLrendererAPI.h"
#include "OpenGLcontext.h"
#include "glm/fwd.hpp"
#include "pch.h"

namespace Engine {

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                           const char *message, const void *userParam) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    CORE_CRITICAL(message);
    return;
  case GL_DEBUG_SEVERITY_MEDIUM:
    CORE_ERROR(message);
    return;
  case GL_DEBUG_SEVERITY_LOW:
    CORE_WARN(message);
    return;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    CORE_TRACE(message);
    return;
  }

  CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLrendererAPI::init() {
#ifdef DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
}

void OpenGLrendererAPI::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLrendererAPI::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLrendererAPI::setViewport(uint32_t width, uint32_t height) {
  glViewport(0, 0, width, height);
}

void OpenGLrendererAPI::drawIndexed(const Ref<VertexArray> &vertexArray) {
  glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLrendererAPI::drawArrays(const Ref<VertexArray> &vertexArray, uint32_t count) {
  glDrawArrays(GL_TRIANGLES, 0, count);
}

}  // namespace Engine
