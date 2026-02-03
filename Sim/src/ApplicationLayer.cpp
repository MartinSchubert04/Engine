#include "ApplicationLayer.h"
#include "Core/Application.h"
#include "Core/Base.h"
#include "Core/DeltaTime.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Planet.h"
#include "Renderer/Buffer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "glm/detail/qualifier.hpp"
#include "imgui.h"
#include "pch.h"
#include <vector>
#include "Sphere.h"

ApplicationLayer::ApplicationLayer() : Layer("App layer") {

  mSkybox.loadCubeMap(std::vector<std::string>({"Sim/Assets/textures/px.jpg", "Sim/Assets/textures/nx.jpg",
                                                "Sim/Assets/textures/py.jpg", "Sim/Assets/textures/ny.jpg",
                                                "Sim/Assets/textures/pz.jpg", "Sim/Assets/textures/nz.jpg"}));

  // mSphere = Planet(1, glm::vec2(32, 32), glm::vec3(0, 0, 0), 1);
  mSphere = Sphere(10);

  mShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/model.vs", "Sim/Assets/shaders/model.fs");
  mSkyboxShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/skybox.vs", "Sim/Assets/shaders/skybox.fs");
  mSkyboxShader->setInt("skybox", 0);

  float aspect = (float)Application::get().getWindow().getWidth() / (float)Application::get().getWindow().getHeight();

  mCamera = Engine::createScope<Engine::Camera>(glm::vec3(0, 0, 1), 45.0f, aspect, 0.1f, 1000);
  mLight = Engine::createRef<Engine::Light>();
}

void ApplicationLayer::onUpdate(Engine::DeltaTime dt) {
  mDeltaTime = dt;

  Engine::RenderCommand::setClearColor({.2, .2, .2, 1});
  Engine::RenderCommand::clear();

  Engine::Renderer::beginScene();
  glEnable(GL_DEPTH_TEST);

  mShader->bind();
  mCamera->update(mShader.get());

  mLight->update(mShader.get());
  mSphere.draw(mShader);

  glDepthFunc(GL_LEQUAL);
  mCamera->update(mSkyboxShader.get());
  mSkyboxShader->bind();
  mSkyboxShader->setMat4("projection", mCamera->getProjection());
  mSkyboxShader->setMat4("view", glm::mat4(glm::mat3(mCamera->getViewMatrix())));
  mSkybox.draw();
  glDepthFunc(GL_LESS);

  Engine::Renderer::endScene();
}

void ApplicationLayer::onImGuiRender() {
  ImGuiIO io = ImGui::GetIO();

  static float timeAccum = 0.0f;
  static int frameCount = 0;
  static int fps = 0;

  timeAccum += io.DeltaTime;
  frameCount++;

  if (timeAccum >= 1.0f) {
    fps = frameCount;
    frameCount = 0;
    timeAccum -= 1.0f;  // o = 0.0f
  }

  if (mActivateWireFrame) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  ImGui::Begin("Scene");
  ImGui::Text("FPS: %d", fps);
  ImGui::Checkbox("Wireframe", &mActivateWireFrame);
  ImGui::End();

  ImGui::Begin("Viewport");
  ImGui::GetMainViewport();
  ImGui::End();
}

void ApplicationLayer::onAttach() {

  Engine::FrameBufferSpecification fbSpec;
  fbSpec.height = 1280;
  fbSpec.width = 720;
  mFrameBuffer = FrameBuffer::create(fbSpec);
}

void ApplicationLayer::onDetach() {}

void ApplicationLayer::onEvent(Engine::Event &e) {

  Engine::EventDispatcher dispatcher(e);

  dispatcher.dispatch<Engine::KeyPressedEvent>(BIND_FN(ApplicationLayer::onKeyPressedEvent));
  dispatcher.dispatch<Engine::MouseMovedEvent>(BIND_FN(ApplicationLayer::onMouseMoved));
  dispatcher.dispatch<Engine::WindowResizeEvent>(BIND_FN(ApplicationLayer::onWindowResize));
}

bool ApplicationLayer::onKeyPressedEvent(Engine::KeyPressedEvent &event) {
  if (event.getKeyCode() == Engine::Key::Escape) {
    onClose();
  }
  if (event.getKeyCode() == Engine::Key::W) {
    mCamera->onMouseWheel(-mCamera->speed * 10 * mDeltaTime.getSeconds());
  }
  if (event.getKeyCode() == Engine::Key::S) {
    mCamera->onMouseWheel(mCamera->speed * 10 * mDeltaTime.getSeconds());
  }

  return false;
}

bool ApplicationLayer::onMouseMoved(Engine::MouseMovedEvent &event) {
  if (Engine::Input::isMouseButtonPressed(Engine::Mouse::ButtonRight)) {
    mCamera->onMouseMove(event.getX(), event.getY(), Engine::Mouse::ButtonRight);
  }

  return false;
}

bool ApplicationLayer::onWindowResize(WindowResizeEvent &event) {
  mCamera->setAspect((float)event.getWidth() / (float)event.getHeight());

  return false;
}

void ApplicationLayer::onClose() {
  // this event only triggers when the key event ocurred with in the app layer
  Engine::WindowCloseEvent e;
  Engine::Application::get().onEvent(e);
}
