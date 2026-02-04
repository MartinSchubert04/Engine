#include "ApplicationLayer.h"
#include "Engine.h"
#include "Events/ApplicationEvent.h"
#include "Planet.h"
#include "Renderer/Buffer.h"
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

  mShader = Renderer::getShaderLibrary().load("Sim/Assets/shaders/model.vs", "Sim/Assets/shaders/model.fs");
  mSkyboxShader = Renderer::getShaderLibrary().load("Sim/Assets/shaders/skybox.vs", "Sim/Assets/shaders/skybox.fs");
  mSkyboxShader->setInt("skybox", 0);

  float aspect = (float)Application::get().getWindow().getWidth() / (float)Application::get().getWindow().getHeight();

  mCamera = Engine::createScope<Engine::Camera>(glm::vec3(0, 0, 1), 45.0f, aspect, 0.1f, 1000);
  mCamera->setDistance(.1);
  mLight = Engine::createRef<Engine::Light>();
}

void ApplicationLayer::onAttach() {

  Engine::FrameBufferSpecification fbSpec;
  fbSpec.width = 1600;
  fbSpec.height = 900;
  mFrameBuffer = Engine::FrameBuffer::create(fbSpec);
}

void ApplicationLayer::onDetach() {}

void ApplicationLayer::onUpdate(Engine::DeltaTime dt) {

  PROFILE_SCOPE("ApplicationLayer::onUpdate");

  mDeltaTime = dt;

  mFrameBuffer->bind();

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

  mFrameBuffer->unbind();
}

void ApplicationLayer::onImGuiRender() {
  ImGuiIO io = ImGui::GetIO();

  // Create the docking environment
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                 ImGuiWindowFlags_NoBackground;

  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
  ImGui::PopStyleVar(3);

  ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");

  ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

  ImGui::End();
  // end of Root docking env, all ImGui widget must go under this code

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

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
  ImGui::Begin("Viewport");
  ImVec2 viewportSize = ImGui::GetContentRegionAvail();  // Obtiene el tamaño de la ventana de ImGui
  uint64_t textureID = mFrameBuffer->getColorAttachmentID();
  ImGui::Image((void *)(textureID), ImVec2{viewportSize.x, viewportSize.y}, ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::Begin("Stats");
  for (auto &r : Instrumentor::getProfileResults())
    ImGui::Text("%.3fms %s", r.time, r.name);
  ImGui::End();
  Instrumentor::clearResults();
}

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
