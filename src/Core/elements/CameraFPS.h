
#include "common.h"

enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

class CameraFPS {

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
  // Constructors
  CameraFPS(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
            float pitch = PITCH);

  CameraFPS(float posX, float posY, float posZ, float upX, float upY, float upZ,
            float yaw, float pitch);

  // // Camera methods
  glm::mat4 getViewMatrix();

  void processKeyboard(Camera_Movement direction, float deltaTime);
  void processMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true);
  void processMouseScroll(float yoffset);

private:
  void updateCameraVectors();
};
