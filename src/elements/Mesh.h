#pragma once

#include "Shader.h"
#include "Vertex.h"
#include "renderer/VertexArray.h"

using namespace render;

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  ~Mesh();

  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;

  void draw(Shader &shader);

private:
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  unsigned int VAO, VBO, EBO;

  void setupMesh();
};
