#pragma once

#include "common.h"

enum InputType { iLeft = 0, iRight = 1, iMiddle = 2, inone = 9 };

class Input {
public:
  static InputType getInputPressed(GLFWwindow *window) {
    InputType result = InputType::inone;

    if (glfwGetMouseButton(window, 0) == GLFW_PRESS)
      return InputType::iLeft;
    else if (glfwGetMouseButton(window, 1) == GLFW_PRESS)
      return InputType::iRight;
    else if (glfwGetMouseButton(window, 2) == GLFW_PRESS)
      return InputType::iMiddle;

    return InputType::inone;
  }
};
