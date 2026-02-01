#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine {
class OpenGLrendererAPI : public RendererAPI {

  void setClearColor(const glm::vec4 &color) override;
  void clear() override;

  void drawIndexed(const Ref<VertexArray> &vertexArray) override;
  void drawArrays(const Ref<VertexArray> &vertexArray, uint32_t count) override;
};

}  // namespace Engine
