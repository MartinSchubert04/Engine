#include <Engine.h>
#include "Engine/Core/EntryPoint.h"
#include "ApplicationLayer.h"
class SandBox : public Engine::Application {

public:
  SandBox() { pushLayer(new ApplicationLayer()); }
  ~SandBox() {}
};

Engine::Application *Engine::createApplication() {
  return new SandBox();
}
