#pragma once

// Std includes
#include <string>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

// GL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "utils/Error.h"

#define BIND_FN(fn) \
  [this](auto &&...args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }

#define DEBUG

#ifdef DEBUG
#define GLCall(x) \
  GLClearError(); \
  x; \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif
