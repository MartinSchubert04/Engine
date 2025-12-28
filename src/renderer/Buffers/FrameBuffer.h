#pragma once

#include "renderer/Render.h"

class FrameBuffer : render::Buffer {

private:
  uint32_t mFBO = mRendererID;
  uint32_t mTexId = 0;
  uint32_t mDepthId = 0;
  int32_t mWidth = 0;
  int32_t mHeight = 0;

public:
  FrameBuffer() : mFBO(0), mDepthId(0) {}

  void create(int32_t width, int32_t height);
  void bind() const override;
  void unbind() const override;
  void destroy() override;
  uint32_t getTexture() const;
};
