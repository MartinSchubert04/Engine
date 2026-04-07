#include <Engine.h>
#include "Engine/Core/EntryPoint.h"
#include "ApplicationLayer.h"
class App : public Engine::Application {

public:
  App() { pushLayer(new ApplicationLayer()); }
  ~App() {}
};

Engine::Application *Engine::createApplication() {
  return new App();
}
