#include "Window.h"
#include "Base.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Engine {

Scope<Window> Window::create(const WindowProps &props) {
#ifdef PLATFORM_WINDOWS
  return createScope<WindowsWindow>(props);
#else
  CORE_ASSERT(false, "Unknown platform!");
  return nullptr;
#endif
}

}  // namespace Engine
