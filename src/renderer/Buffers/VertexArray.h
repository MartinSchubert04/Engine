#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace render {
class VertexArray {
private:
  unsigned int mVAO;

public:
  VertexArray();

  void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

  void bind() const;
  void unbind() const;
  void destroy();
};

}  // namespace render
