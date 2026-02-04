#include "OpenGLbuffer.h"
#include "Renderer/Buffer.h"
#include "pch.h"

namespace Engine {

//////////////////////////////////////////////////////////
// Vertex Buffer /////////////////////////////////////////
//////////////////////////////////////////////////////////

OpenGLvertexBuffer::OpenGLvertexBuffer(std::vector<Vertex> vertices) {

  glCreateBuffers(1, &mRendererID);
  glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

OpenGLvertexBuffer::OpenGLvertexBuffer(std::vector<float> vertices) {
  glCreateBuffers(1, &mRendererID);
  glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

OpenGLvertexBuffer::~OpenGLvertexBuffer() {
  glDeleteBuffers(1, &mRendererID);
}

void OpenGLvertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
}

void OpenGLvertexBuffer::unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////
// Index Buffer //////////////////////////////////////////
//////////////////////////////////////////////////////////

OpenGLindexBuffer::OpenGLindexBuffer(std::vector<uint32_t> &indices) : mCount(indices.size()) {

  glGenBuffers(1, &mRendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}

OpenGLindexBuffer::~OpenGLindexBuffer() {
  glDeleteBuffers(1, &mRendererID);
}

void OpenGLindexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
}

void OpenGLindexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace Engine
