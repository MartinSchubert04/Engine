#include "OpenGLframeBuffer.h"
#include "Core/Assert.h"
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

}  // namespace Engine
