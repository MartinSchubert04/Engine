#include "Engine.h"

class SandBox : public Engine::Application {

public:
  SandBox() { pushLayer(new Engine::ImGuiLayer); }
  ~SandBox() {}
};

Engine::Application *Engine::createApplication() {
  return new SandBox();
}
