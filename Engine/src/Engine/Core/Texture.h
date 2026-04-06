#pragma once

#include "pch.h"
#include <cstdint>

namespace Engine {

class Texture {

public:
  virtual ~Texture() = default;

  virtual void bind(unsigned int slot = 0) const = 0;
  virtual void unbind() const = 0;

  virtual inline int getWidth() const = 0;
  virtual inline int getHeight() const = 0;
  virtual inline std::string getType() const = 0;
  virtual inline std::string getPath() const = 0;
  virtual inline uint32_t getID() const = 0;

  virtual inline void setType(std::string type) = 0;
  virtual inline bool isValid() const = 0;

  static Ref<Texture> create2D(const std::string &path);
  static Ref<Texture> createCubeMap(const std::vector<std::string> &faces);
};

class Texture2D : public Texture {
public:
  Texture2D(const std::string &path);
  ~Texture2D() override;

  void bind(uint32_t slot = 0) const override;
  void unbind() const override;

  inline int getWidth() const override { return mWidth; };
  inline int getHeight() const override { return mHeight; };
  inline std::string getType() const override { return mType; };
  inline std::string getPath() const override { return mFilepath; };
  virtual inline uint32_t getID() const override { return mID; };

  inline void setType(std::string type) override { mType = type; };
  inline bool isValid() const override { return mID != 0; }

private:
  std::string mFilepath;
  unsigned char *mLocalBuff;
  uint32_t mID;
  std::string mType;
  int mWidth, mHeight, mBPP;
};

class TextureCubeMap : public Texture {
public:
  TextureCubeMap(const std::vector<std::string> &faces);

  void bind(uint32_t slot = 0) const override;
  void unbind() const override;

  uint32_t getID() const override { return mID; }
  int getWidth() const override { return mWidth; }
  int getHeight() const override { return mHeight; }
  std::string getType() const override { return mType; }
  std::string getPath() const override { return mFilepath; }

  void setType(std::string type) override { mType = type; }
  bool isValid() const override { return mID != 0; }

private:
  std::string mFilepath;
  unsigned char *mLocalBuff;
  uint32_t mID;
  std::string mType;
  int mWidth, mHeight, mBPP;
};

}  // namespace Engine
