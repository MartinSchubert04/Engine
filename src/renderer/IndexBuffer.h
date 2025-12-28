#pragma once

#include "Render.h"

namespace render {

class IndexBuffer : Buffer {

private:
  GLuint &mIBO = mRendererID;

public:
  void create(const std::vector<unsigned int> &indices);
  void bind() const override;
  void unbind() const override;
  void destroy() override;
};

};  // namespace render
