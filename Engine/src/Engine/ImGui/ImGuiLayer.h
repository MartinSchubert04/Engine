#pragma once
#include "pch.h"
#include "Core/Layer.h"
#include "Events/Event.h"
#include "Events/MouseEVent.h"
#include "Events/KeyEVent.h"
#include "Events/ApplicationEvent.h"

namespace Engine {

class ImGuiLayer : public Layer {

public:
  ImGuiLayer() : Layer("ImGui") {}

  ~ImGuiLayer();

  void onAttach() override;
  void onDetach() override;
  void onImGuiRender() override;
  void onEvent(Event &event) override;

  void blockEvents(bool block) { mBlockEvents = block; }

  void begin();
  void end();

private:
  bool onMouseButtonPressedEvent(MouseButtonPressedEvent &e);
  bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent &e);
  bool onMouseMovedEvent(MouseMovedEvent &e);
  bool onMouseScrolledEvent(MouseScrolledEvent &e);
  bool onKeyPressedEvent(KeyPressedEvent &e);
  bool onKeyReleasedEvent(KeyReleasedEvent &e);
  bool onKeyTypedEvent(KeyTypedEvent &e);
  bool onWindowResizeEvent(WindowResizeEvent &e);
  bool onWindowCloseEvent(WindowCloseEvent &e);

private:
  float mTime = (float)glfwGetTime();
  bool mBlockEvents = true;
};

}  // namespace Engine
