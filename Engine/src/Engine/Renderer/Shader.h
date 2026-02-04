
#pragma once
#include "Core/Base.h"
#include "pch.h"
#include <string>

namespace Engine {

class Shader {

public:
  Shader(const std::string &vertexPath, const std::string &fragPath);
  ~Shader();

  void bind();
  void unbind();

  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w) const;
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  void checkCompileErrors(GLuint shader, std::string type);

private:
  uint32_t mRendererID;
};

class ShaderLibrary {

public:
  void add(const Ref<Shader> shader);
  Ref<Shader> load(const std::string &filepath);
  Ref<Shader> load(const std::string &name, const std::string &filepath);
  Ref<Shader> get(const Ref<Shader> shader);

private:
  std::unordered_map<class Kty, class Ty> mShaders;
};

}  // namespace Engine
