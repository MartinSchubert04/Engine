#pragma once
#include "Core/Base.h"
#include "Core/DeltaTime.h"
#include <Engine.h>
#include "Core/Light.h"
#include "Planet.h"
class ApplicationLayer : public Engine::Layer {

public:
  ApplicationLayer();

  void onUpdate(Engine::DeltaTime dt) override;
  void onEvent(Engine::Event &e) override;

private:
  bool onKeyPressedEvent(Engine::KeyPressedEvent &e);
  bool onMouseMoved(Engine::MouseMovedEvent &event);
  bool onWindowResize(WindowResizeEvent &ew);
  void onClose();

private:
  Engine::Ref<Engine::Shader> mShader;
  Engine::Ref<Engine::Shader> mSkyboxShader;
  Engine::Skybox mSkybox;

  Engine::Ref<Engine::VertexArray> vertexArray;
  Engine::Ref<Engine::VertexBuffer> vertexBuffer;
  Engine::Ref<Engine::IndexBuffer> indexBuffer;
  Engine::Scope<Engine::Camera> mCamera;
  Engine::Ref<Engine::Light> mLight;

  Planet mSphere;

  Engine::DeltaTime mDeltaTime;
};
