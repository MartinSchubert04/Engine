#include "Texture.h"
#include "Core/Log.h"
#include "pch.h"

namespace Engine {

// ── Factory ───────────────────────────────────────────

Ref<Texture> Texture::create2D(const std::string &path) {
  return createRef<Texture2D>(path);
}

Ref<Texture> Texture::createCubeMap(const std::vector<std::string> &faces) {
  return createRef<TextureCubeMap>(faces);
}

//  ── Texture2D ───────────────────────────────────────────

Texture2D::Texture2D(const std::string &path) : mFilepath(path), mLocalBuff(nullptr), mWidth(0), mHeight(0), mBPP(0) {

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

Texture2D::~Texture2D() {
  if (mID != 0) {
    glDeleteTextures(1, &mID);
    mID = 0;
  }
}

void Texture2D::bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, mID);
}

void Texture2D::unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

//  ── TextureCubeMap ───────────────────────────────────────────

TextureCubeMap::TextureCubeMap(const std::vector<std::string> &faces) {
  glGenTextures(1, &mID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

  stbi_set_flip_vertically_on_load(false);
  int w, h, channels;
  for (uint32_t i = 0; i < faces.size(); i++) {
    unsigned char *data = stbi_load(faces[i].c_str(), &w, &h, &channels, 0);
    GLenum format = channels == 4 ? GL_RGBA : channels == 3 ? GL_RGB : GL_RED;

    if (data) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      CORE_ERROR("Failed to load cubemap face: {0}", faces[i]);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubeMap::bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
}

void TextureCubeMap::unbind() const {
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}  // namespace Engine
