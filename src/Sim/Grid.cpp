#include "Grid.h"

Grid::Grid(glm::vec2 size, glm::vec3 position, float squareSize)
    : size(size), position(position), squareSize(squareSize) {

  // offset helps to center the grid
  float offset_X = (size.x - 1) * squareSize / 2.0f;
  float offset_Z = (size.y - 1) * squareSize / 2.0f;

  for (unsigned int z{0}; z < size.y; z++) {
    for (unsigned int x{0}; x < size.x; x++) {
      Vertex vertex;

      vertex.position.y = position.y;

      vertex.position.x = (x * squareSize) - offset_X;
      vertex.position.z = (z * squareSize) - offset_Z;

      vertex.normal = {.0f, 1.f, .0f};

      vertex.texCoords = glm::vec2(0.0f, 0.0f);
      vertex.color = glm::vec4(color, 1);

      vertices.push_back(vertex);
    }
  }

  for (unsigned int z = 0; z < size.y; z++) {
    for (unsigned int x = 0; x < size.x - 1; x++) {
      // multiply by size.x "skips" a whole row then set the vertex in that
      // position
      indices.push_back(z * size.x + x);
      indices.push_back(z * size.x + (x + 1));
    }
  }

  for (unsigned int x = 0; x < size.x; x++) {
    for (unsigned int z = 0; z < size.y - 1; z++) {
      indices.push_back(z * size.x + x);
      indices.push_back((z + 1) * size.x + x);
    }
  }

  mesh = std::make_unique<Mesh>(vertices, indices,
                                std::vector<std::shared_ptr<Texture>>{});
}

void Grid::draw(Shader *shader) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  shader->setMat4("model", model);
  shader->setVec4("modelColor", glm::vec4(color, 1));

  mesh->draw(*shader, DrawType::LINES);
}

void Grid::update(glm::vec3 force, float delta) {}
