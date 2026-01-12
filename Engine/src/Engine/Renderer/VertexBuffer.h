#pragma once

#include "renderer/Render.h"

namespace render {

class VertexBuffer : public Buffer {

private:
  GLuint &mVBO = mRendererID;

public:
  virtual void create(const std::vector<Vertex> &vertices);
  virtual void bind() const override;
  virtual void unbind() const override;
  virtual void destroy() override;
};

}  // namespace render
