#pragma once
#include "pch.h"
#include "Window.h"
#include "Base.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Core/LayerStack.h"

namespace Engine {

// father class for client applications
class Application {

public:
  Application();
  virtual ~Application();

  void run();
  void onEvent(Event &e);

  bool onWindowClose(WindowCloseEvent &e);
  bool onWindowResize(WindowResizeEvent &e);

  void pushLayer(Layer *layer);
  void pushOverlay(Layer *layer);

  Window &getWindow() { return *mWindow; }
  static Application &get() { return *s_instance; }

private:
  Scope<Window> mWindow;  // only this class owns it
  bool mRunning = true;

  LayerStack mLayerStack;

  static Application *s_instance;
};

Application *createApplication();

}  // namespace Engine
