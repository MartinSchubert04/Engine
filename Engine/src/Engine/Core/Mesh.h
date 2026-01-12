#pragma once

#include "Shader.h"
#include "Vertex.h"
#include "Renderer/VertexArray.h"
#include "Texture.h"

using namespace render;

enum class DrawType { TRIANGLES, LINES, LINE_STRIP };
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures);
  ~Mesh();

  void draw(Shader &shader, DrawType type = DrawType::TRIANGLES);

  // Getters
  inline size_t getVertexCount() const { return vertices.size(); }
  inline size_t getIndexCount() const { return indices.size(); }
  inline size_t getTextureCount() const { return textures.size(); }

private:
  VertexBuffer vb;
  IndexBuffer ib;
  VertexArray va;

  void setupMesh();
};
