#include "Planet.h"
#include "common.h"
#include "glm/fwd.hpp"
#include <vector>

Planet::Planet(float radius, glm::vec2 segments, glm::vec3 pos, float mass)
    : radius(radius), segments(segments), position(pos), scaleFactor(1),
      speed(0.0f), acceleration(0.0f), mass(mass) {

  float PI = 3.14159265359f;
  this->color = glm::vec4(.8, .8, .8, .8);

  for (int y{0}; y <= segments.y; y++) {
    for (int x{0}; x <= segments.x; x++) {
      Vertex vertex;

      float xSeg = float(x) / segments.x;
      float ySeg = float(y) / segments.y;

      float xPos = std::cos(xSeg * 2.0f * PI) * std::sin(ySeg * PI);
      float yPos = std::cos(ySeg * PI);
      float zPos = std::sin(xSeg * 2.0f * PI) * std::sin(ySeg * PI);

      vertex.position = glm::vec3(xPos, yPos, zPos);

      // calculate normal
      vertex.normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
      vertex.color = color;

      vertices.push_back(vertex);
    }
  }

  for (int y = 0; y < segments.y; y++) {
    for (int x = 0; x < segments.x; x++) {
      unsigned int curRow = y * (segments.x + 1);
      unsigned int nextRow = (y + 1) * (segments.x + 1);

      // Triángulo 1
      indices.push_back(curRow + x);
      indices.push_back(nextRow + x);
      indices.push_back(curRow + x + 1);

      // Triángulo 2
      indices.push_back(nextRow + x);
      indices.push_back(nextRow + x + 1);
      indices.push_back(curRow + x + 1);
    }
  }

  mesh = std::make_unique<Mesh>(vertices, indices,
                                std::vector<std::shared_ptr<Texture>>{});
}

void Planet::draw(Shader *shader) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model = glm::scale(model, glm::vec3(radius));

  shader->setMat4("model", model);
  shader->setVec4("modelColor", color);

  mesh->draw(*shader);
}

void Planet::update(glm::vec3 force, float delta) {
  applyForce(force);
  updatePhysics(delta);
}

void Planet::scale(float scalar) {
  scaleFactor = scalar;
}

std::vector<glm::vec3> Planet::getVerticesPos() {
  std::vector<glm::vec3> positions;
  for (auto &v : vertices) {
    positions.push_back(v.position * radius);
  }

  return positions;
}

void Planet::applyForce(glm::vec3 force) {
  acceleration += force / mass;
}

void Planet::updatePhysics(float delta) {
  speed += acceleration * delta;
  position += speed * delta;

  acceleration = glm::vec3(0.0f);
}

void Planet::checkCollision(float distance) {
  float currentDist = glm::distance(this->position, glm::vec3(0.0f));
  float limit = distance - this->radius;

  float energyLoss = 0.1;

  if (currentDist > limit) {
    glm::vec3 collisionNormal = glm::normalize(this->position);

    this->position = collisionNormal * limit;

    this->speed = glm::reflect(this->speed, -collisionNormal);
    this->speed *= (1.0f - energyLoss);
  }
}
