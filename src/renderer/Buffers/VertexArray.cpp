#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "common.h"

namespace render {

VertexArray::VertexArray() {
  GLcall(glGenVertexArrays(1, &mVAO));
}

void VertexArray::addBuffer(const VertexBuffer &vb,
                            const VertexBufferLayout &layout) {
  bind();
  vb.bind();
  const auto &elements = layout.getElements();
  unsigned int offset = 0;

  for (unsigned int i{0}; i < elements.size(); i++) {

    const auto &element = elements[i];
    GLcall(glEnableVertexAttribArray(i));
    GLcall(glVertexAttribPointer(i, element.count, element.type,
                                 element.normalized, layout.getStride(),
                                 (const void *)(uintptr_t)offset));

    offset += element.count * VertexBufferElement::getSizeOf(element.type);
  }
}

void VertexArray::bind() const {
  GLcall(glBindVertexArray(mVAO));
}

void VertexArray::unbind() const {
  GLcall(glBindVertexArray(0));
}

void VertexArray::destroy() {
  GLcall(glDeleteVertexArrays(1, &mVAO));
}
}  // namespace render
