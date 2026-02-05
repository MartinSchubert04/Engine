#include "Camera.h"

namespace Engine {

void Camera::updateViewMatrix() {
  mPosition = focusPoint - getForward() * distance;

  glm::quat orientation = getDirection();
  mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4(orientation);
  mViewMatrix = glm::inverse(mViewMatrix);
}

}  // namespace Engine
