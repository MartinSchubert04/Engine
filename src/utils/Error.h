#pragma once
#include "common.h"

static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

static bool GLLogCall(const char *fun, const char *file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "\n [ OPENGL ERORR ] :: (" << error << "): " << fun
              << " File: " << file << ":" << line << "\n\n";

    return false;
  }

  return true;
}
