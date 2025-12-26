#pragma once

#include "Render.h"

namespace render {
class GL_VertexIndexBuffer : VertexIndexBuffer {

private:
  unsigned int mRedererID;
  unsigned int mVAO, mVBO, mEBO;

public:
  GL_VertexIndexBuffer(const std::vector<Vertex> &vertices,
                       const std::vector<unsigned int> &indices);
  ~GL_VertexIndexBuffer();

  void bind() const override;
  void unbind() const override;
  void destroy() override;
};

};  // namespace render
