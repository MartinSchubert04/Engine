#include "System.h"
#include "Object.h"

namespace universe {

System::System() {
  int ballCount = 2;
  float ballSize = 0.1;

  for (int i{0}; i < ballCount; i++) {
    int factor = i * 200;

    std::unique_ptr<Planet> sphere =
        std::make_unique<Planet>(ballSize + ((float)i / 2), glm::vec2(25, 25),
                                 glm::vec3{-1 + i, -1 + i, -1 + i}, 1 + factor);
    addEntity(std::move(sphere));
  }
}

void System::update(glm::vec3 force, float delta) {
  for (int i{0}; i < planets.size(); i++) {
    auto p = planets[i].get();

    p->update(force, delta);

    for (int j = i + 1; j < planets.size(); j++) {
      mSolver.solveCollitions(p, planets[j].get());
      mSolver.solveGravity(p, planets[j].get());
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

}  // namespace universe
