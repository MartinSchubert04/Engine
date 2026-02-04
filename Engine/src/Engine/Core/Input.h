#pragma once

#include "Events/KeyCodes.h"
#include "Events/MouseCodes.h"
#include "pch.h"

namespace Engine {

class Input {

public:
  virtual ~Input() = default;

  inline static bool isKeyPressed(KeyCode keycode) { return s_Instance->isKeyPressedImpl(keycode); }
  inline static bool isMouseButtonPressed(MouseCode keycode) { return s_Instance->isMouseButtonPressedImpl(keycode); }
  inline static glm::vec2 getMousePos() { return s_Instance->getMousePosImpl(); }
  inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
  inline static bool getMouseY() { return s_Instance->getMouseYImpl(); }

protected:
  virtual bool isKeyPressedImpl(KeyCode keycode) = 0;
  virtual bool isMouseButtonPressedImpl(MouseCode keycode) = 0;
  virtual glm::vec2 getMousePosImpl() = 0;
  virtual float getMouseXImpl() = 0;
  virtual float getMouseYImpl() = 0;

private:
  static Scope<Input> s_Instance;
};

}  // namespace Engine
