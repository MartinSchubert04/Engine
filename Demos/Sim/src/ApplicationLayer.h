#pragma once

#include <Engine.h>
#include <vector>
#include "Core/Light.h"
#include "Planet.h"
#include "Sphere.h"
class ApplicationLayer : public Engine::Layer {

public:
  ApplicationLayer();

  void onUpdate(Engine::DeltaTime dt) override;
  void onEvent(Engine::Event &e) override;
  void onImGuiRender() override;
  void onAttach() override;
  void onDetach() override;

private:
  bool onKeyPressedEvent(Engine::KeyPressedEvent &e);
  bool onMouseMoved(Engine::MouseMovedEvent &event);
  bool onWindowResize(WindowResizeEvent &ew);
  void onClose();

private:
  Engine::Ref<Engine::Shader> mPlanetShader;
  Engine::Ref<Engine::Shader> mSkyboxShader;
  Engine::Skybox mSkybox;

  Engine::Ref<Engine::FrameBuffer> mFrameBuffer;
  Engine::CameraController mCameraController;
  Engine::Ref<Engine::Light> mLight;
  Engine::DeltaTime mDeltaTime;

  glm::vec2 mViewportSize = {1600.0f, 900.0f};

  // Planet mSphere;
  Sphere mSphere;

  bool mActivateWireFrame = false;
};
