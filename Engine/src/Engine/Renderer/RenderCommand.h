#pragma once

#include "Core/Base.h"
#include "RendererAPI.h"
#include "VertexArray.h"

namespace Engine {

class RenderCommand {
public:
  inline static void setClearColor(const glm::vec4 &color) { s_RendererAPI->setClearColor(color); }
  inline static void clear() { s_RendererAPI->clear(); }

  inline static void drawIndexed(const Ref<VertexArray> &vertexArray) { s_RendererAPI->drawIndexed(vertexArray); }
  inline static void drawArrays(const Ref<VertexArray> &vertexArray, uint32_t count) {
    s_RendererAPI->drawArrays(vertexArray, count);
  }

private:
  static Scope<RendererAPI> s_RendererAPI;
};

}  // namespace Engine
