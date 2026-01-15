#include "Engine.h"

class SandBox : public Engine::Application {

public:
  SandBox() {}
  ~SandBox() {}
};

int main() {
  Engine::Log::init();
  auto app = Engine::createApplication();
  E_INFO("App created");
  app->run();
  delete app;
}

Engine::Application *Engine::createApplication() {
  return new SandBox();
}
