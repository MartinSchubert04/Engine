#pragma once
#include "Window.h"
#include "Base.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Core/LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Platform/OpenGL/OpenGLbuffer.h"

namespace Engine {

// father class for client applications
class Application {

public:
  Application();
  virtual ~Application();

  void run();
  void onEvent(Event &e);

  void pushLayer(Layer *layer);
  void pushOverlay(Layer *layer);

  Window &getWindow() { return *mWindow; }
  static Application &get() { return *s_instance; }

private:
  bool onWindowClose(WindowCloseEvent &e);
  bool onWindowResize(WindowResizeEvent &e);

private:
  Scope<Window> mWindow;  // only this class owns it
  bool mRunning = true;
  ImGuiLayer *mImGuiLayer;
  LayerStack mLayerStack;
  bool mMinimized = false;

  float mLastFrameTime = 0.0f;

  static Application *s_instance;
};

Application *createApplication();

}  // namespace Engine
