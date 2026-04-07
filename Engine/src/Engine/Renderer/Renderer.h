#pragma once
#include "Core/Base.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "RendererAPI.h"
#include "VertexArray.h"
namespace Engine {

class Renderer {
public:
  static void init();
  static void shutdown();

  static void beginScene();
  static void endScene();
  static void onWindowResize(uint32_t width, uint32_t height);

  static void submit(const Ref<VertexArray> &vertexArray);
  static void submitArrays(const Ref<VertexArray> &vertexArray, uint32_t count);

  inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
  inline static ShaderLibrary getShaderLibrary() { return RendererAPI::getShaderLibrary(); }
};

}  // namespace Engine
