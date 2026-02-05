#include "CameraController.h"
#include "Core/Base.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/MouseCodes.h"
#include "Events/MouseEVent.h"

namespace Engine {

CameraController::CameraController(float aspect) : mCamera(glm::vec3(0, 0, 1), 45.0f, aspect, 0.1f, 1000) {}

void CameraController::onUpdate(DeltaTime dt) {
  mDelta = dt;

  if (Input::isKeyPressed(Engine::Key::W)) {
    onMouseWheel(-mCamera.scrollSpeed * mDelta.getSeconds());
  }
  if (Input::isKeyPressed(Engine::Key::S)) {
    onMouseWheel(mCamera.scrollSpeed * mDelta.getSeconds());
  }
  if (Input::isKeyPressed(Engine::Key::R)) {
    reset();
  }
}

void CameraController::updateShader(Ref<Shader> shader) {
  shader->setMat4("view", mCamera.getViewMatrix());
  shader->setMat4("projection", mCamera.getProjection());
  shader->setVec3("camPos", mCamera.getPosition());
}

void CameraController::onEvent(Event &e) {

  EventDispatcher dispatcher(e);

  dispatcher.dispatch<WindowResizeEvent>(BIND_FN(CameraController::onWindowResize));
  dispatcher.dispatch<MouseScrolledEvent>(BIND_FN(CameraController::onMouseScroll));
  dispatcher.dispatch<MouseMovedEvent>(BIND_FN(CameraController::onMouseMoved));
}

bool CameraController::onMouseMoved(MouseMovedEvent &e) {
  glm::vec2 pos2d{e.getX(), e.getY()};

  if (Input::isMouseButtonPressed(Mouse::ButtonRight)) {
    glm::vec2 delta = (pos2d - mCamera.getPosition2D()) * mDelta.getSeconds();

    float sign = mCamera.getUp().y < 0 ? -1.0f : 1.0f;
    mCamera.yaw += sign * delta.x * cRotationSpeed;
    mCamera.pitch += delta.y * cRotationSpeed;

    mCamera.updateViewMatrix();
  } else if (Input::isMouseButtonPressed(Mouse::ButtonLeft)) {
    // TODO: Adjust pan speed for distance|
    glm::vec2 delta = (pos2d - mCamera.getPosition2D()) * mCamera.displaceSpeed * mDelta.getSeconds();

    mCamera.focusPoint += -mCamera.getRight() * delta.x * mCamera.distance;
    mCamera.focusPoint += mCamera.getUp() * delta.y * mCamera.distance;

    mCamera.updateViewMatrix();
  }

  mCamera.setPosition(pos2d);
  return false;
}

void CameraController::reset() {
  mCamera.focusPoint = {0.0f, 0.0f, 0.0f};
  // distance = 5.0f;
  mCamera.updateViewMatrix();
}

bool CameraController::onWindowResize(WindowResizeEvent &e) {
  mCamera.setAspect((float)e.getWidth() / (float)e.getHeight());
  return false;
}
bool CameraController::onMouseScroll(MouseScrolledEvent &e) {
  onMouseWheel(mCamera.scrollSpeed * e.getOffsetY());
  return false;
}
void CameraController::onMouseWheel(double offset) {
  mCamera.setDistance(-offset * 0.5f);
  mCamera.updateViewMatrix();
}

}  // namespace Engine
