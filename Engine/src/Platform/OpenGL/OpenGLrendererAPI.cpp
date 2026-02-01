#include "OpenGLrendererAPI.h"
#include "OpenGLcontext.h"
#include "glm/fwd.hpp"
#include "pch.h"

namespace Engine {

void OpenGLrendererAPI::setClearColor(const glm::vec4 &color) {
  glClearColor(color.r, color.g, color.b, color.a);
}
void OpenGLrendererAPI::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLrendererAPI::drawIndexed(const Ref<VertexArray> &vertexArray) {
  glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLrendererAPI::drawArrays(const Ref<VertexArray> &vertexArray, uint32_t count) {
  glDrawArrays(GL_TRIANGLES, 0, count);
}

}  // namespace Engine
