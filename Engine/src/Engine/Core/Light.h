#pragma once

#include "pch.h"

#include "Shader.h"

class Light {
public:
  Light() {
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    position = {-0.2f, -1.0f, -0.3f};
    strength = 70.0f;
  }

  ~Light() {}

  void update(Shader *shader) {

    shader->setVec3("dirLight.direction", position);
    shader->setVec3("dirLight.ambient", color * 0.4f);
    shader->setVec3("dirLight.diffuse", color * 0.4f);
    shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  }

  glm::vec3 position;
  glm::vec3 color;
  float strength;
};
