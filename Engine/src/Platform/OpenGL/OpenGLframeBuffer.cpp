#include "OpenGLframeBuffer.h"
#include "Core/Assert.h"
#include "Renderer/FrameBuffer.h"
#include "pch.h"

namespace Engine {

static const uint32_t s_MaxFramebufferSize = 8192;

OpenGLframeBuffer::OpenGLframeBuffer(const FrameBufferSpecification &spec) : mSpecification(spec) {
  invalidate();
}

OpenGLframeBuffer::~OpenGLframeBuffer() {
  glDeleteFramebuffers(1, &mRendererID);
}

void OpenGLframeBuffer::invalidate() {

  if (mRendererID) {
    glDeleteFramebuffers(1, &mRendererID);
    glDeleteTextures(1, &mColorAttachment);
    glDeleteTextures(1, &mDepthAttachment);

    mColorAttachment = 0;
    mDepthAttachment = 0;
  }

  glCreateFramebuffers(1, &mRendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

  glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
  glBindTexture(GL_TEXTURE_2D, mColorAttachment);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mSpecification.width, mSpecification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

  glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
  glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mSpecification.width, mSpecification.height);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mSpecification.width, mSpecification.height, 0,
  // GL_DEPTH_STENCIL,
  //              GL_UNSIGNED_INT_24_8, NULL);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

  CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete")

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLframeBuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
  glViewport(0, 0, mSpecification.width, mSpecification.height);
}

void OpenGLframeBuffer::unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLframeBuffer::resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
    CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
    return;
  }
  mSpecification.width = width;
  mSpecification.height = height;

  invalidate();
}

}  // namespace Engine
