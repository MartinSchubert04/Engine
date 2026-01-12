#pragma once

#include "pch.h"
#include "Core/Vertex.h"
#include "Core/Mesh.h"

class Object {

protected:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

public:
  std::unique_ptr<Mesh> mesh;

  virtual void update(glm::vec3 force, float delta) = 0;
  virtual void draw(Shader *shader) = 0;
};
