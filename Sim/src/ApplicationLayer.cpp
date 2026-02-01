#include "ApplicationLayer.h"
#include "Core/DeltaTime.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Planet.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "glm/detail/qualifier.hpp"
#include "pch.h"
#include <vector>

ApplicationLayer::ApplicationLayer() : Layer("App layer") {

  mSkybox.loadCubeMap(std::vector<std::string>({"Sim/Assets/textures/px.jpg", "Sim/Assets/textures/nx.jpg",
                                                "Sim/Assets/textures/py.jpg", "Sim/Assets/textures/ny.jpg",
                                                "Sim/Assets/textures/pz.jpg", "Sim/Assets/textures/nz.jpg"}));

  mSphere = Planet(1, glm::vec2(12, 12), glm::vec3(0, 0, 0), 1);

  mShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/triangle.vs", "Sim/Assets/shaders/triangle.fs");
  mSkyboxShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/skybox.vs", "Sim/Assets/shaders/skybox.fs");
  mSkyboxShader->setInt("skybox", 0);

  mCamera = Engine::createScope<Engine::Camera>(glm::vec3(0, 0, 1), 45.0f, 1.3f, 0.1f, 1000);
  // mCamera = Engine::createScope<Engine::CameraFPS>(glm::vec3(0, 0, 5));
}

void ApplicationLayer::onUpdate(Engine::DeltaTime dt) {
  mDeltaTime = dt;

  Engine::RenderCommand::setClearColor({.2, .2, .2, 1});
  Engine::RenderCommand::clear();

  Engine::Renderer::beginScene();
  glEnable(GL_DEPTH_TEST);

  mShader->bind();
  mCamera->update(mShader.get());
  mSphere.draw(mShader);

  glDepthFunc(GL_LEQUAL);
  mCamera->update(mSkyboxShader.get());
  mSkyboxShader->bind();
  mSkyboxShader->setMat4("projection", mCamera->getProjection());
  mSkyboxShader->setMat4("view", glm::mat4(glm::mat3(mCamera->getViewMatrix())));
  mSkybox.draw();
  glDepthFunc(GL_LESS);

  // Engine::Renderer::submit(vertexArray);

  Engine::Renderer::endScene();
}

void ApplicationLayer::onEvent(Engine::Event &e) {

  Engine::EventDispatcher dispatcher(e);

  dispatcher.dispatch<Engine::KeyPressedEvent>(BIND_FN(ApplicationLayer::onKeyPressedEvent));
  dispatcher.dispatch<Engine::MouseMovedEvent>(BIND_FN(ApplicationLayer::onMouseMoved));
  // dispatcher.dispatch<Engine::WindowResizeEvent>(BIND_FN(ApplicationLayer::onWindowResize));
}

bool ApplicationLayer::onKeyPressedEvent(Engine::KeyPressedEvent &event) {
  if (event.getKeyCode() == Engine::Key::Escape) {
    onClose();
  }
  if (event.getKeyCode() == Engine::Key::W) {
    mCamera->onMouseWheel(-mCamera->speed * mDeltaTime.getSeconds());
  }
  if (event.getKeyCode() == Engine::Key::S) {
    mCamera->onMouseWheel(mCamera->speed * mDeltaTime.getSeconds());
  }
  // if (event.getKeyCode() == Engine::Key::D) {
  //   mCamera->Position.x += mCamera->MovementSpeed * mDeltaTime.getSeconds();
  // }
  // if (event.getKeyCode() == Engine::Key::A) {
  //   mCamera->Position.x -= mCamera->MovementSpeed * mDeltaTime.getSeconds();
  // }

  return false;
}

bool ApplicationLayer::onMouseMoved(Engine::MouseMovedEvent &event) {
  // if (Engine::Input::isMouseButtonPressed(Engine::Mouse::ButtonRight)) {
  //   mCamera->onMouseMove(event.getX(), event.getY(), Engine::Mouse::ButtonRight);
  // }

  return false;
}

bool ApplicationLayer::onWindowResize(WindowResizeEvent &e) {
  mCamera->setAspect((float)e.getWidth() / (float)e.getHeight());
  return false;
}

void ApplicationLayer::onClose() {
  // this event only triggers when the key event ocurred with in the app layer
  Engine::WindowCloseEvent e;
  Engine::Application::get().onEvent(e);
}
