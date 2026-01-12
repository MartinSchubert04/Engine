
#include "pch.h"
#include "IndexBuffer.h"

namespace render {

void IndexBuffer::create(const std::vector<unsigned int> &indices) {
  GLcall(glGenBuffers(1, &mIBO));
  GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
  GLcall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW));
}

void IndexBuffer::bind() const {
  GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO));
}

void IndexBuffer::unbind() const {
  GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::destroy() {
  GLcall(glDeleteBuffers(1, &mIBO));
}
}  // namespace render
