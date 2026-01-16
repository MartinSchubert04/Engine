#include "pch.h"
#include "Core/Layer.h"

namespace Engine {

class ImGuiLayer : public Layer {

public:
  ImGuiLayer() : Layer("ImGui") {}

  ~ImGuiLayer();

  void onAttach() override;
  void onDetach() override;
  void onUpdate() override;
  void onEvent(Event &event) override;

private:
  float mTime = (float)glfwGetTime();
};

}  // namespace Engine
