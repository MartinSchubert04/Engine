#pragma once
#include "Renderer/FrameBuffer.h"

namespace Engine {

class OpenGLframeBuffer : public FrameBuffer {
public:
  OpenGLframeBuffer(const FrameBufferSpecification &spec);
  ~OpenGLframeBuffer();

  void bind() const override;
  void unbind() const override;
  const FrameBufferSpecification &getSpecification() const override;

  void invalidate();

private:
  uint32_t mRendererID;
  uint32_t mColorAttachment;
  uint32_t mDepthAttachment;

  FrameBufferSpecification mSpecification;
};
}  // namespace Engine
