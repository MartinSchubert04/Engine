#include "Engine.h"

class SandBox : public Engine::Application {

public:
  SandBox() {}
  ~SandBox() {}
};

int main() {
  Engine::Log::init();
  E_INFO("INIT");
  auto app = Engine::createApplication();
  E_INFO("App creada");
  app->run();
  delete app;
}

Engine::Application *Engine::createApplication() {
  return new SandBox();
}
