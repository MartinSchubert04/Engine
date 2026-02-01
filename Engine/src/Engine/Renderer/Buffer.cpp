#include "Buffer.h"
#include "Core/Assert.h"
#include "Core/Base.h"
#include "Platform/OpenGL/OpenGLbuffer.h"
#include "Renderer.h"
#include "RendererAPI.h"
#include <vector>

namespace Engine {

Ref<VertexBuffer> VertexBuffer::create(std::vector<Vertex> vertices) {

  switch (Renderer::getAPI()) {
  case RendererAPI::API::None: {
    CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
    return nullptr;
    break;
  }

  case RendererAPI::API::OpenGL: {
    return createRef<OpenGLvertexBuffer>(vertices);
    break;
  }
  }

  CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
  return nullptr;
}

Ref<VertexBuffer> VertexBuffer::create(std::vector<float> vertices) {

  switch (Renderer::getAPI()) {
  case RendererAPI::API::None: {
    CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
    return nullptr;
    break;
  }

  case RendererAPI::API::OpenGL: {
    return createRef<OpenGLvertexBuffer>(vertices);
    break;
  }
  }

  CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
  return nullptr;
}

Scope<IndexBuffer> IndexBuffer::create(std::vector<uint32_t> &indices) {

  switch (Renderer::getAPI()) {
  case RendererAPI::API::None: {
    CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
    return nullptr;
    break;
  }

  case RendererAPI::API::OpenGL: {
    return createScope<OpenGLindexBuffer>(indices);
    break;
  }
  }

  CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
  return nullptr;
}

}  // namespace Engine
