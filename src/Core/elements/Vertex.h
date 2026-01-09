#pragma once
#include "common.h"

struct Vertex {

  Vertex() = default;
  Vertex(glm::vec3 pos) { position = pos; };
  Vertex(glm::vec3 pos, glm::vec3 norm) {
    position = pos;
    normal = norm;
  };
  Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 textureCoords) {
    position = pos;
    normal = norm;
    texCoords = textureCoords;
  };

  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
  glm::vec4 color;
  float useDiffuseTexture;
};

struct Point {
  Point() = default;

  glm::vec3 position;
};
