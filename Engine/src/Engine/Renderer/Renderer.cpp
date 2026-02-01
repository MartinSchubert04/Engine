#include "Renderer.h"

namespace Engine {

void Renderer::beginScene() {}

void Renderer::endScene() {}

void Renderer::submit(const Ref<VertexArray> &vertexArray) {
  vertexArray->bind();
  RenderCommand::drawIndexed(vertexArray);
}

void Renderer::submitArrays(const Ref<VertexArray> &vertexArray, uint32_t count) {
  vertexArray->bind();
  RenderCommand::drawArrays(vertexArray, count);
}

}  // namespace Engine
