#pragma once
#include "common.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec4 color;
  float useDiffuseTexture;
};
