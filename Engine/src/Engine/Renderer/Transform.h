#pragma once
#include "Core/Base.h"
#include "pch.h"
#include "Renderer/Shader.h"

namespace Engine {

class Transform {
public:
  Transform() { mModel = glm::mat4(1.0f); }

  void scale(float value) { mModel = glm::scale(mModel, glm::vec3(value)); }

  void translate(glm::vec3 position) { mModel = glm::translate(mModel, position); }

  void rotate(float angle, glm::vec3 axis) { mModel = glm::rotate(mModel, glm::radians(angle), axis); }

  void setModel(Ref<Shader> shader, std::string name = "model") { shader->setMat4(name, mModel); }

  void reset() { mModel = glm::mat4(1.0f); }

private:
  glm::mat4 mModel;
};
}  // namespace Engine
