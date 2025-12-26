
#include "common.h"
#include "VertexIndexBuffer.h"

using namespace render;

GL_VertexIndexBuffer::GL_VertexIndexBuffer(
    const std::vector<Vertex> &vertices,
    const std::vector<unsigned int> &indices) {
  glGenVertexArrays(1, &mVAO);

  glGenBuffers(1, &mEBO);
  glGenBuffers(1, &mVBO);

  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, normal));

  glBindVertexArray(0);
}

GL_VertexIndexBuffer::~GL_VertexIndexBuffer() {
  destroy();
}

void GL_VertexIndexBuffer::bind() const {
  glBindVertexArray(mVAO);
}

void GL_VertexIndexBuffer::unbind() const {
  glBindVertexArray(0);
}

void GL_VertexIndexBuffer::destroy() {
  glDeleteBuffers(1, &mRedererID);
}
