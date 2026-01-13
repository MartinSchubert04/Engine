#include "Window.h"
#include "Base.h"
#include "Plataform/Windows/WindowsWindow.h"

namespace Engine {

Scope<Window> Window::create(const WindowProps &props) {

#ifdef PLATFORM_WINDOWS
  return createScope<WindowsWindow>(props);
#endif
}

}  // namespace Engine
