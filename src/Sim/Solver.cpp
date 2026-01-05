#include "Solver.h"
#include "common.h"

void Solver::solveCollitions(Planet *self, Planet *other) {

  float minDistance = self->radius + other->radius;
  glm::vec3 relativePos = self->position - other->position;
  float distance = glm::length(relativePos);

  if (distance < minDistance) {
    // 1. Resolver el solapamiento (Static Resolution)
    // Evita que los planetas se queden pegados
    glm::vec3 collisionNormal = relativePos / distance;  // Normalizada
    float overlap = minDistance - distance;

    self->position += collisionNormal * (overlap * 0.5f);
    other->position -= collisionNormal * (overlap * 0.5f);

    // 2. Resolver el rebote (Dynamic Resolution)
    glm::vec3 relativeVelocity = self->speed - other->speed;

    // Calculamos la velocidad a lo largo de la normal
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    // No resolver si las velocidades ya se están alejando
    if (velocityAlongNormal > 0)
      return;

    // Coeficiente de restitución (1.0 = rebote perfecto, 0.5 = amortiguado)
    float restitution = 0.8f;

    float j = -(1.0f + restitution) * velocityAlongNormal;
    j /= (1.0f / self->mass + 1.0f / other->mass);

    // Aplicar impulso
    glm::vec3 impulse = j * collisionNormal;
    self->speed += (1.0f / self->mass) * impulse;
    other->speed -= (1.0f / other->mass) * impulse;
  }
}

void Solver::solveGravity(Planet *self, Planet *other) {}
