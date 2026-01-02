#pragma once

#include "Input.h"
#include "common.h"
#include "Shader.h"

// Defines several possible options for camera movement
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class Camera {
public:
  // Camera Attributes
  glm::vec3 Position;

  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // Euler Angles
  float Yaw;
  float Pitch;

  // Camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  float speed = 10.0f;

  // Constructors
  Camera(glm::vec3 position = glm::vec3(0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);

  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

  Camera(const glm::vec3 &position, float fov, float aspect, float near,
         float far) {
    mPosition = position;
    mAspect = aspect;
    mNear = near;
    mFar = far;
    mFOV = fov;

    setAspect(mAspect);

    updateViewMatrix();
  }

  // // Camera methods
  glm::mat4 GetViewMatrix();

  void ProcessKeyboard(Camera_Movement direction, float deltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);
  void ProcessMouseScroll(float yoffset);

  // ------------------------------
  // NEW CAMERA IMPEMENTATION
  // ------------------------------

  void update(Shader *shader) {
    glm::mat4 model{1.0f};
    shader->setMat4("model", model);
    shader->setMat4("view", mViewMatrix);
    shader->setMat4("projection", getProjection());
    shader->setVec3("camPos", mPosition);
  }

  void setAspect(float aspect) {
    mProjection = glm::perspective(mFOV, aspect, mNear, mFar);
  }

  void setDistance(float offset) {
    mDistance += offset;
    updateViewMatrix();
  }

  const glm::mat4 &getProjection() const { return mProjection; }

  glm::mat4 getViewProjection() const {
    return mProjection * getViewProjection();
  }

  glm::vec3 getUp() const { return getDirection() * cUp; }

  glm::vec3 getRight() const { return getDirection() * cRight; }

  glm::vec3 getForward() const { return getDirection() * cForward; }

  glm::quat getDirection() const {
    return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
  }

  glm::mat4 getViewMatrix() const { return mViewMatrix; }

  void onMouseWheel(double delta) {
    setDistance(delta * 0.5f);

    updateViewMatrix();
  }

  void reset() {
    mFocus = {0.0f, 0.0f, 0.0f};
    // mDistance = 5.0f;
    updateViewMatrix();
  }

  void onMouseMove(double x, double y, InputType button) {
    glm::vec2 pos2d{x, y};

    if (button == InputType::iRight) {
      glm::vec2 delta = (pos2d - mCurrentPos2d) * 0.004f;

      float sign = getUp().y < 0 ? -1.0f : 1.0f;
      mYaw += sign * delta.x * cRotationSpeed;
      mPitch += delta.y * cRotationSpeed;

      updateViewMatrix();
    } else if (button == InputType::iMiddle) {
      // TODO: Adjust pan speed for distance
      glm::vec2 delta = (pos2d - mCurrentPos2d) * 0.003f;

      mFocus += -getRight() * delta.x * mDistance;
      mFocus += getUp() * delta.y * mDistance;

      updateViewMatrix();
    }

    mCurrentPos2d = pos2d;
  }

  void updateViewMatrix() {
    mPosition = mFocus - getForward() * mDistance;

    glm::quat orientation = getDirection();
    mViewMatrix =
        glm::translate(glm::mat4(1.0f), mPosition) * glm::mat4(orientation);
    mViewMatrix = glm::inverse(mViewMatrix);
  }

private:
  void updateCameraVectors();

  glm::mat4 mViewMatrix;
  glm::mat4 mProjection = glm::mat4{1.0f};
  glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};

  glm::vec3 mFocus = {0.0f, 0.0f, 0.0f};

  float mDistance = 5.0f;
  float mAspect;
  float mFOV;
  float mNear;
  float mFar;
  float mPitch = 0.0f;
  float mYaw = 0.0f;

  glm::vec2 mCurrentPos2d = {0.0f, 0.0f};

  const glm::vec3 cRight = {1.0f, 0.0f, 0.0f};
  const glm::vec3 cUp = {0.0f, 1.0f, 0.0f};
  const glm::vec3 cForward = {0.0f, 0.0f, -1.0f};

  const float cRotationSpeed = 2.0f;
};
