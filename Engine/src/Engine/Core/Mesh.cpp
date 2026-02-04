#include "Mesh.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "pch.h"
#include "Renderer/Renderer.h"

namespace Engine {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ref<Texture>> textures) {

  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
}

void Mesh::setupMesh() {

  va = VertexArray::create();
  va->bind();

  vb = VertexBuffer::create(vertices);
  ib = IndexBuffer::create(indices);

  vb->bind();
  ib->bind();

  Engine::BufferLayout layout = {
      {Engine::Types::ShaderDataType::float3, "aPos"},       {Engine::Types::ShaderDataType::float3, "aNormal"},
      {Engine::Types::ShaderDataType::float2, "aTexCoords"}, {Engine::Types::ShaderDataType::float4, "aColor"},
      {Engine::Types::ShaderDataType::float1, "UseTexture"},
  };

  vb->setLayout(layout);

  va->addVertexBuffer(vb);
  va->setIndexBuffer(ib);
}

void Mesh::draw(Shader &shader, DrawType type) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++) {

    std::string number;
    std::string name = textures[i]->getType();
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);

    shader.setInt(("material." + name + number).c_str(), i);
    textures[i]->bind(i);
  }

  // GLcall(glActiveTexture(GL_TEXTURE0));

  Renderer::submit(va);

  // va->bind();

  // switch (type) {

  // case DrawType::TRIANGLES:
  //   glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  //   break;
  // case DrawType::LINES:
  //   glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
  //   break;
  // case DrawType::LINE_STRIP:
  //   glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
  //   break;
  // default:
  //   glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  // }

  // va.unbind(); no need to loose copute time on this call
}

}  // namespace Engine
