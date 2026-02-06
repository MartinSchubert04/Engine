#pragma once
#include "Core/DeltaTime.h"
#include "Core/Layer.h"
#include "Engine/ImGui/ImGuiLayer.h"

namespace Engine {

class EditorLayer : public Layer {

public:
  EditorLayer();
  virtual ~EditorLayer() = default;

  void onAttach() override;
  void onDetach() override;
  void onUpdate(DeltaTime dt) override;
  void onImGuiRender() override;
  void onEvent(Event &event) override;

private:
  bool mActivateWireframe;
};
}  // namespace Engine
