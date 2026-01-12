#pragma once

#include "pch.h"
#include "Core/Shader.h"
#include "Object.h"

class Grid : Object {

public:
  glm::vec2 size;
  glm::vec3 position;
  glm::vec3 squareSize;
  glm::vec3 color = GRID_COLOR;

  Grid(glm::vec2 size, glm::vec3 position = {0, 0, 0}, float squareSize = 2);

  void draw(Shader *shader) override;
  void update(glm::vec3 force, float delta) override;
};

class Grid3D {

public:
  Grid3D(glm::vec3 size, glm::vec3 position, float squareLenght);
};
