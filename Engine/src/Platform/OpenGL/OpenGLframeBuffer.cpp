#include "OpenGLframeBuffer.h"
#include "Renderer/FrameBuffer.h"
#include "pch.h"

namespace Engine {

OpenGLframeBuffer::OpenGLframeBuffer(const FrameBufferSpecification &spec) : mSpecification(spec) {
  invalidate();
}

OpenGLframeBuffer::~OpenGLframeBuffer() {
  glDeleteFramebuffers(1, &mRendererID);
}

void OpenGLframeBuffer::invalidate() {

  glCreateBuffers(1, &mRendererID);
  glBindFramebuffer(GL_TEXTURE_2D, mRendererID);

  glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
  glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mSpecification.width, mSpecification.height, 0, GL_RGBA, GL_UNSIGNED_INT,
               nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE2, mColorAttachment, 0);

  glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
  glBindTexture(GL_TEXTURE_2D, mDepthAttachment);

  glBindFramebuffer(0, mRendererID);
}

}  // namespace Engine
