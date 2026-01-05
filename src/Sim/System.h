#pragma once

#include "Solver.h"
#include "Planet.h"

class System {

private:
  Solver mSolver;

public:
  std::vector<std::unique_ptr<Planet>> planets;

  void addEntity(std::unique_ptr<Planet> planet);
  void draw(Shader *shader);
  void update(glm::vec3 force, float delta);

  void setCount();
  void setSize(float size);
};
