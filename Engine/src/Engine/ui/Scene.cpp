#include "Scene.h"
#include "pch.h"
#include "Core/Input.h"
#include "imgui/imgui.h"
#include <memory>

namespace UI {

void Scene::resize(int32_t width, int32_t height) {
  mSize.x = width;
  mSize.y = height;

  mFrameBuffer->create((int32_t)mSize.x, (int32_t)mSize.y);
}

void Scene::onMouseMove(double x, double y, InputType button) {
  mCamera->onMouseMove(x, y, button);
}

void Scene::onMouseWheel(double delta) {
  mCamera->onMouseWheel(delta);
}

void Scene::loadModel(const std::string &filepath) {
  mModel = std::make_unique<Model>(filepath);
}

void Scene::render(float delta) {

  mShader->use();

  mCamera->setAspect(mSize.x / mSize.y);
  mCamera->update(mShader.get());
  mLight->update(mShader.get());

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  mFrameBuffer->bind();

  if (mModel) {
    mModel->update(mShader.get());
    mModel->draw(*mShader.get());
  }

  glm::vec3 gravity = {0.f, 0, 0.f};

  for (auto &contBound : mContainerBoudaries) {
    contBound->draw(mShader.get());
  }

  grid.draw(mShader.get());

  mSystem->update(gravity, delta);
  for (auto &p : mSystem->planets) {
    p->checkCollision(mContainer->radius);
  }
  mSystem->draw(mShader.get());

  mFrameBuffer->unbind();

  ImGui::Begin("Scene");

  ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
  mSize = {viewportPanelSize.x, viewportPanelSize.y};

  // add rendered texture to ImGUI scene window
  uint64_t textureID = mFrameBuffer->getTexture();
  ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{mSize.x, mSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

  ImGui::End();
}

}  // namespace UI
