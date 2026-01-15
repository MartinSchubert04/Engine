#pragma once
#include "pch.h"
#include "Window.h"
#include "Base.h"
#include "Events/Event.h"

namespace Engine {

// father class for client applications
class Application {

public:
  Application();
  virtual ~Application();

  void run();
  void onEvent(Event &e);

private:
  Scope<Window> mWindow;  // only this class owns it
  bool mRunning = true;
};

Application *createApplication();

}  // namespace Engine
