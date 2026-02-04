#pragma once
#include "Core/Base.h"
#include "pch.h"
#include "Renderer/Shader.h"

namespace Engine {

class Transform {
public:
  Transform() { mModel = glm::mat4(1.0f); }

  void scale(float value) { glm::scale(mModel, glm::vec3(value)); }

  void translate(glm::vec3 position) { glm::translate(mModel, position); }

  void setModel(Ref<Shader> shader, std::string name = "model") { shader->setMat4(name, mModel); }

  void reset() { mModel = glm::mat4(1.0f); }

private:
  glm::mat4 mModel;
};
}  // namespace Engine
