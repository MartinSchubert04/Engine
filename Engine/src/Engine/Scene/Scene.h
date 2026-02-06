#pragma once
#include "entt/entt.hpp"

namespace Engine {

class Scene {

public:
  Scene();
  ~Scene();

  entt::entity createEntity();

  void onUpdate();

private:
  entt::registry mRegistry;
};
}  // namespace Engine
