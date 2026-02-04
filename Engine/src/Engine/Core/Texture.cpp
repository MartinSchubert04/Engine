#include "Texture.h"
#include "Core/Log.h"
#include "pch.h"

namespace Engine {

Texture::Texture(std::string path) : mFilepath(path), mLocalBuff(nullptr), mWidth(0), mHeight(0), mBPP(0) {

  stbi_set_flip_vertically_on_load(true);

  glGenTextures(1, &mID);
  glBindTexture(GL_TEXTURE_2D, mID);

  mLocalBuff = stbi_load(mFilepath.c_str(), &mWidth, &mHeight, &mBPP, 4);

  if (mLocalBuff) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalBuff);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(mLocalBuff);
  } else {
    CORE_ERROR("Texture failed to load at path: {0}", path);
  }
}

Texture::~Texture() {
  if (mID != 0) {
    glDeleteTextures(1, &mID);
    mID = 0;
  }
}

void Texture::bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace Engine
