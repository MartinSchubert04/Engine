#pragma once

#include "Core/PlataformDetection.h"

#ifdef PLATAFORM_WINDOWS
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#endif
#include "Core/Base.h"
#include "Core/Log.h"

// Std includes
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
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

#ifdef PLATAFORM_WINDOWS
  #include <Windows.h>
#endif

#define BLACK glm::vec3(0, 0, 0)
#define WHITE glm::vec3(1, 1, 1)
#define RED glm::vec3(1, 0, 0)
#define BLUE glm::vec3(0, 0, 1)
#define GREEN glm::vec3(0, 1, 0)
#define YELLOW glm::vec3(1, 1, 0)
#define LIGHT_BLUE glm::vec3(0, 1, 1)
#define PURPLE glm::vec3(1, 0, 1)
#define GREY glm::vec3(0.25f)
// #define GRID_COLOR glm::vec3(0.509, 0.333, 0.490) * 0.5f
#define GRID_COLOR glm::vec3(.8, .8, .8)
