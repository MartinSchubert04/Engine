#include "Renderer.h"
namespace Engine {

void Renderer::beginScene() {}

void Renderer::endScene() {}

void Renderer::onWindowResize(uint32_t width, uint32_t height) {
  RenderCommand::setViewport(width, height);
}

void Renderer::submit(const Ref<VertexArray> &vertexArray) {
  vertexArray->bind();
  RenderCommand::drawIndexed(vertexArray);
}

void Renderer::submitArrays(const Ref<VertexArray> &vertexArray, uint32_t count) {
  vertexArray->bind();
  RenderCommand::drawArrays(vertexArray, count);
}

}  // namespace Engine
