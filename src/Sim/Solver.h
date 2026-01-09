#pragma once

#include "Planet.h"

class Solver {

public:
  void solveCollitions(Planet *self, Planet *other);
  void solveGravity(Planet *self, Planet *other);
};
