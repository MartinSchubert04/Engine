#pragma once

#include "Solver.h"
#include "Planet.h"

namespace universe {

static float Gconstant = 1.f;

class System {

private:
  Solver mSolver;

public:
  System();

  std::vector<std::unique_ptr<Planet>> planets;

  void addEntity(std::unique_ptr<Planet> planet);
  void draw(Shader *shader);
  void update(glm::vec3 force, float delta);

  void setCount();
  void setSize(float size);

  std::vector<glm::vec3> getProjectionOrbit(Planet *target, float delta,
                                            unsigned int steps = 1000) {
    std::vector<glm::vec3> projection;

    // Estado virtual inicial
    glm::vec3 vPos = target->position;
    glm::vec3 vVel = target->speed;

    for (int i = 0; i < steps; ++i) {
      glm::vec3 totalForce(0.f);

      for (auto &p : planets) {
        if (p.get() == target)
          continue;

        glm::vec3 diff = p->position - vPos;
        float distSqr = glm::dot(diff, diff) + 0.1f;
        float forceMag =
            (universe::Gconstant * target->mass * p->mass) / distSqr;
        totalForce += glm::normalize(diff) * forceMag;
      }

      vVel += (totalForce / target->mass) * delta;
      vPos += vVel * delta;

      projection.push_back(vPos);
    }
    return projection;
  }
};

}  // namespace universe
