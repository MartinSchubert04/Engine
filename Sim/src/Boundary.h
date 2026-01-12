#pragma once

#include "Core/Shader.h"
#include "Core/Vertex.h"
#include "Core/Mesh.h"

enum class BoundaryType { SPHERE, CUBE, RECTANGLE };

class Boundary {
protected:
  std::vector<Vertex> mVertices;
  std::vector<unsigned int> mIndices;
  std::unique_ptr<Mesh> mMesh;

public:
  float size;
  BoundaryType type;

  Boundary(float size, BoundaryType type);

  virtual void draw(Shader *shader) = 0;
};

class BoundarySphere : public Boundary {

  void draw(Shader *shader) override;
};

class BoundaryRec : public Boundary {

  void draw(Shader *shader) override;
};
