#pragma once

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Vertex.h"
#include "Texture.h"
#include "Renderer/Buffer.h"

using namespace Engine;

namespace Engine {

enum class DrawType { TRIANGLES, LINES, LINE_STRIP };
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Ref<Texture>> textures;

  Mesh() = default;
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture>> textures = {});

  void draw(Shader &shader, DrawType type = DrawType::TRIANGLES);

  // Getters
  inline size_t getVertexCount() const { return vertices.size(); }
  inline size_t getIndexCount() const { return indices.size(); }
  inline size_t getTextureCount() const { return textures.size(); }

private:
  Ref<VertexBuffer> vb;
  Ref<IndexBuffer> ib;
  Ref<VertexArray> va;

  void setupMesh();
};

}  // namespace Engine
