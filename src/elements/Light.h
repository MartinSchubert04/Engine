#pragma once

#include "common.h"

#include "Shader.h"

class Light {
public:
  Light() {
    color = glm::vec3(-0.2f, -1.0f, -0.3f);
    position = {1.5f, 3.5f, 3.0f};
    strength = 100.0f;
  }

  ~Light() {}

  void update(Shader *shader) {

    shader->setVec3("dirLight.direction", position);
    shader->setVec3("dirLight.ambient", color * strength);
  }

  glm::vec3 position;

  glm::vec3 color;

  float strength;
};
