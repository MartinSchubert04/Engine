#include "Solver.h"
#include "pch.h"
#include "System.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

void Solver::solveCollitions(Planet *self, Planet *other) {

  float minDistance = self->radius + other->radius;
  glm::vec3 relativePos = self->position - other->position;
  float distance = glm::length(relativePos);

  if (distance < minDistance) {

    glm::vec3 collisionNormal = relativePos / distance;  // Normalizada
    float overlap = minDistance - distance;

    self->position += collisionNormal * (overlap * 0.5f);
    other->position -= collisionNormal * (overlap * 0.5f);

    // 2.Dynamic Resolution
    glm::vec3 relativeVelocity = self->speed - other->speed;

    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    // skip if they already going away from each other
    if (velocityAlongNormal > 0)
      return;

    float restitution = 0.8f;

    float j = -(1.0f + restitution) * velocityAlongNormal;
    j /= (1.0f / self->mass + 1.0f / other->mass);

    // apply impulse
    glm::vec3 impulse = j * collisionNormal;
    self->speed += (1.0f / self->mass) * impulse;
    other->speed -= (1.0f / other->mass) * impulse;
  }
}

void Solver::solveGravity(Planet *self, Planet *other) {
  glm::vec3 diff = other->position - self->position;

  float minDistance = self->radius + other->radius + 0.0000001;
  float distance = glm::length(diff);

  if (minDistance > distance)
    return;

  float distanceSqr = distance * distance;

  glm::vec3 forceDir = glm::normalize(diff);

  glm::vec3 force = forceDir * (universe::Gconstant * (self->mass * other->mass) / distanceSqr);  // avoid div by 0

  self->applyForce(force);
  other->applyForce(-force);
}
