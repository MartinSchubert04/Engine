#include "FrameBuffer.h"
#include "pch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLframeBuffer.h"
namespace Engine {

static Ref<FrameBuffer> create(FrameBufferSpecification &spec) {
  switch (Renderer::getAPI()) {
  case RendererAPI::API::None: {
    CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
    return nullptr;
    break;
  }

  case RendererAPI::API::OpenGL: {
    return createRef<OpenGLframeBuffer>(spec);
    break;
  }
  }

  CORE_ASSERT(false, "Renderer API: returning nullptr (NO RENDERER API PROVIDED)");
  return nullptr;
};

}  // namespace Engine
