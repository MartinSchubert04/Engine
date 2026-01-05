#pragma once

#include "Object.h"
#include "elements/Mesh.h"

class Planet : public Object {
public:
  float radius;
  glm::vec2 segments;
  glm::vec3 position;
  glm::vec4 color;
  float scaleFactor;

  float mass;
  glm::vec3 speed;
  glm::vec3 acceleration;

  Planet(float radius, glm::vec2 segments, glm::vec3 position, float mass = 0);

  void draw(Shader *shader) override;

  void update(glm::vec3 force, float delta) override;
  void applyForce(glm::vec3 force);
  void updatePhysics(float delta);
  void checkCollision(float distance);

  void scale(float scalar);

  Mesh *getMesh() { return mesh.get(); }
  std::vector<glm::vec3> getVerticesPos();
};
