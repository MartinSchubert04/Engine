#include "FrameBuffer.h"
#include "common.h"

namespace render {

void FrameBuffer::create(int32_t width, int32_t height) {
  mWidth = width;
  mHeight = height;

  if (mFBO) {
    destroy();
  }

  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  // Create color texture
  glCreateTextures(GL_TEXTURE_2D, 1, &mTexId);
  glBindTexture(GL_TEXTURE_2D, mTexId);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, mWidth, mHeight);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mTexId, 0);

  // Create depth-stencil texture
  glCreateTextures(GL_TEXTURE_2D, 1, &mDepthId);
  glBindTexture(GL_TEXTURE_2D, mDepthId);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mWidth, mHeight);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, mDepthId, 0);

  GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, buffers);

  unbind();
}

void FrameBuffer::destroy() {
  if (mFBO) {
    glDeleteFramebuffers(1, &mFBO);
    if (mTexId) {
      glDeleteTextures(1, &mTexId);
    }
    if (mDepthId) {
      glDeleteTextures(1, &mDepthId);
    }
    mFBO = 0;
    mTexId = 0;
    mDepthId = 0;
  }
}

void FrameBuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glViewport(0, 0, mWidth, mHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t FrameBuffer::getTexture() const {
  return mTexId;
}

}  // namespace render
