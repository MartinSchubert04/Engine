#pragma once
#include "Renderer/FrameBuffer.h"

namespace Engine {

class OpenGLframeBuffer : public FrameBuffer {
public:
  OpenGLframeBuffer(const FrameBufferSpecification &spec);
  ~OpenGLframeBuffer();

  void bind() const override;
  void unbind() const override;
  void resize(uint32_t width, uint32_t height) override;
  const FrameBufferSpecification &getSpecification() const override { return mSpecification; };

  uint32_t getColorAttachmentID() override { return mColorAttachment; };

  void invalidate();

private:
  uint32_t mRendererID;
  uint32_t mColorAttachment, mDepthAttachment;

  FrameBufferSpecification mSpecification;
};
}  // namespace Engine
