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
#include <chrono>

// GL includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/plugins/ImFileBrowser.h"

#include "stb/stb_image.h"

#include "utils/Error.h"

#define BIND_FN(fn) \
  [this](auto &&...args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }
