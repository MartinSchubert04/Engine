#pragma once

#include "Events/KeyCodes.h"
#include "Events/MouseCodes.h"
#include "Input.h"
#include "Renderer/Shader.h"

// Defines several possible options for camera movement

namespace Engine {
class Camera {
public:
  float scrollSpeed = 100.0f;
  float distance = 5.0f;
  float pitch = 0.0f;
  float yaw = 0.0f;
  glm::vec3 mFocus = {0.0f, 0.0f, 0.0f};

  Camera(const glm::vec3 &position, float fov, float aspect, float nearPlane, float farPlane) {
    mPosition = position;
    mAspect = aspect;
    mNear = nearPlane;
    mFar = farPlane;
    mFOV = fov;

    setAspect(mAspect);

    updateViewMatrix();
  }

  void update(Shader *shader) {
    shader->setMat4("view", mViewMatrix);
    shader->setMat4("projection", getProjection());
    shader->setVec3("camPos", mPosition);
  }

  void updateViewMatrix() {
    mPosition = mFocus - getForward() * distance;

    glm::quat orientation = getDirection();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4(orientation);
    mViewMatrix = glm::inverse(mViewMatrix);
  }

  void setAspect(float aspect) { mProjection = glm::perspective(mFOV, aspect, mNear, mFar); }

  void setProjection(float fov, float aspect, float nearPlane, float farPlane) {
    mProjection = glm::perspective(fov, aspect, nearPlane, farPlane);
  }

  void setPosition(glm::vec2 pos) { mCurrentPos2d = pos; }

  void setDistance(float offset) {
    distance += offset;
    updateViewMatrix();
  }

  float getDistance() { return distance; }
  glm::vec2 getPosition() { return mCurrentPos2d; }

  const glm::mat4 &getProjection() const { return mProjection; }

  glm::vec3 getUp() const { return getDirection() * cUp; }

  glm::vec3 getRight() const { return getDirection() * cRight; }

  glm::vec3 getForward() const { return getDirection() * cForward; }

  glm::quat getDirection() const { return glm::quat(glm::vec3(-pitch, -yaw, 0.0f)); }

  glm::mat4 getViewMatrix() const { return mViewMatrix; }

private:
  glm::mat4 mViewMatrix;
  glm::mat4 mProjection = glm::mat4{1.0f};
  glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};

  float mAspect;
  float mFOV;
  float mNear;
  float mFar;

  glm::vec2 mCurrentPos2d = {0.0f, 0.0f};

  const glm::vec3 cRight = {1.0f, 0.0f, 0.0f};
  const glm::vec3 cUp = {0.0f, 1.0f, 0.0f};
  const glm::vec3 cForward = {0.0f, 0.0f, -1.0f};

  const float cRotationSpeed = 2.0f;
};
}  // namespace Engine
