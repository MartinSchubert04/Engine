#include "RendererAPI.h"
#include "Renderer/Shader.h"

namespace Engine {

RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
ShaderLibrary RendererAPI::s_ShaderLib;

}  // namespace Engine
