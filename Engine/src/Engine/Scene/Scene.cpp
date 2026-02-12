#include "Scene.h"
#include "pch.h"
#include "Components.h"

namespace Engine {

Scene::Scene() {

  entt::entity entity = mRegistry.create();
  mRegistry.emplace<TransformComponent>(entity);

  if (mRegistry.any_of<TransformComponent>(entity)) {
    TransformComponent &transform = mRegistry.get<TransformComponent>(entity);
  }

  auto view = mRegistry.view<TransformComponent>();

  for (auto &entity : view) {
    TransformComponent &transform = mRegistry.get<TransformComponent>(entity);
  }

  auto viewTransforms = mRegistry.view<TransformComponent>();
}

Scene::~Scene() {}

void Scene::onUpdate() {}

entt::entity Scene::createEntity() {
  return mRegistry.create();
};

}  // namespace Engine
