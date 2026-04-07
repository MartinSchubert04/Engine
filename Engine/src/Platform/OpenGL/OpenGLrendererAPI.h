#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine {
class OpenGLrendererAPI : public RendererAPI {

  void init() override;
  void setClearColor(const glm::vec4 &color) override;
  void clear() override;
  void setViewport(uint32_t width, uint32_t height) override;

  void drawIndexed(const Ref<VertexArray> &vertexArray) override;
  void drawArrays(const Ref<VertexArray> &vertexArray, uint32_t count) override;
};

}  // namespace Engine
