#include "System.h"
#include "Object.h"

void System::update(glm::vec3 force, float delta) {
  for (int i{0}; i < planets.size(); i++) {
    auto p = planets[i].get();

    p->update(force, delta);

    for (int j = i + 1; j < planets.size(); j++) {
      mSolver.solveCollitions(p, planets[j].get());
    }
  }
}

void System::draw(Shader *shader) {
  for (auto &p : planets) {
    p->draw(shader);
  }
}

void System::addEntity(std::unique_ptr<Planet> planet) {
  planets.push_back(std::move(planet));
}

void System::setSize(float size) {
  for (auto &p : planets) {
    p->radius = size;
  }
}
