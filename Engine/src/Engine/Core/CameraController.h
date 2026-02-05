#pragma once
#include "Core/Camera.h"
#include "DeltaTime.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEVent.h"

namespace Engine {

class CameraController {

public:
  CameraController(float aspect);

  void onUpdate(DeltaTime dt);
  void updateShader(Ref<Shader> shader);

  void onEvent(Event &e);

  void reset();

  Camera &getCamera() { return mCamera; }

private:
  bool onMouseMoved(MouseMovedEvent &e);
  bool onMouseScroll(MouseScrolledEvent &e);
  bool onWindowResize(WindowResizeEvent &e);
  void onMouseWheel(double offset);

private:
  Camera mCamera;
  DeltaTime mDelta;

  const glm::vec3 cRight = {1.0f, 0.0f, 0.0f};
  const glm::vec3 cUp = {0.0f, 1.0f, 0.0f};
  const glm::vec3 cForward = {0.0f, 0.0f, -1.0f};

  const float cRotationSpeed = 2.0f;
};

}  // namespace Engine
