#include "ApplicationLayer.h"
#include "Core/Base.h"
#include "Engine.h"
#include "Events/ApplicationEvent.h"
#include "Planet.h"
#include "imgui.h"
#include "pch.h"
#include <vector>
#include "Sphere.h"

using TextureArray = std::vector<Engine::Ref<Engine::Texture>>;

ApplicationLayer::ApplicationLayer() : Layer("App layer"), mCameraController(1600.0f / 900.0f) {

  mSkybox.loadCubeMap(
      std::vector<std::string>({"Demos/Sim/Assets/textures/space/px.jpg", "Demos/Sim/Assets/textures/space/nx.jpg",
                                "Demos/Sim/Assets/textures/space/py.jpg", "Demos/Sim/Assets/textures/space/ny.jpg",
                                "Demos/Sim/Assets/textures/space/pz.jpg", "Demos/Sim/Assets/textures/space/nz.jpg"}));

  auto diffuseMap = Engine::createRef<Engine::Texture2D>("Demos/Sim/Assets/textures/earth/colormap/earth_colormap.jpg");
  diffuseMap->setType("texture_diffuse");

  // TextureArray bumpMap = {
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/px.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/nx.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/py.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/ny.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/pz.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/bump/nz.png"),
  // };
  // TextureArray specularMap = {
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/px.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/nx.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/py.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/ny.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/pz.png"),
  //     Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/specular/nz.png"),
  // };
  // for (auto &t : specularMap) {
  //   t->setType("texture_specular");
  // }
  // for (auto &t : bumpMap) {
  //   t->setType("texture_bump");
  // }

  TextureArray cloudMap = {
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/px.png"),
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/nx.png"),
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/py.png"),
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/ny.png"),
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/pz.png"),
      Engine::Texture::create2D("Demos/Sim/Assets/textures/earth/cloud/nz.png"),
  };
  for (auto &t : cloudMap) {
    t->setType("texture_diffuse");
  }

  mEarth = Sphere(0.5f, {12, 12}, {0, 0, 0},
                  {
                      1.0f,  // size
                      10.0f,  // rotationSpeed
                      23.5f,  // angleTilt (realista)
                      0.0f  // orbitSpeed (si no orbitás)
                  });

  mEarthClouds = Sphere(100);
  mEarthClouds.setProperties({1.02f,  // un poco mas que el de la tierra
                              12.0f,  // rotationSpeed (un poco más rápido)
                              23.5f,  // mismo tilt
                              0.0f});

  mEarth.setTexture(diffuseMap);
  mEarthClouds.setTexturesCubeSphere(cloudMap);

  mCloudShader =
      Renderer::getShaderLibrary().load("Demos/Sim/Assets/shaders/cloud.vs", "Demos/Sim/Assets/shaders/cloud.fs");

  mPlanetShader =
      Renderer::getShaderLibrary().load("Demos/Sim/Assets/shaders/planet.vs", "Demos/Sim/Assets/shaders/planet.fs");

  mSkyboxShader =
      Renderer::getShaderLibrary().load("Demos/Sim/Assets/shaders/skybox.vs", "Demos/Sim/Assets/shaders/skybox.fs");
  mSkyboxShader->bind();
  mSkyboxShader->setInt("skybox", 0);

  float aspect = (float)Application::get().getWindow().getWidth() / (float)Application::get().getWindow().getHeight();

  mCameraController.getCamera().setDistance(.1);
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

  auto spec = mFrameBuffer->getSpecification();
  if (spec.width != (uint32_t)mViewportSize.x || spec.height != (uint32_t)mViewportSize.y) {
    mFrameBuffer->resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
    mCameraController.getCamera().setAspect(mViewportSize.x / mViewportSize.y);
  }

  mCameraController.onUpdate(dt);

  mDeltaTime = dt;

  mFrameBuffer->bind();

  Engine::RenderCommand::setClearColor({.2, .2, .2, 1});
  Engine::RenderCommand::clear();

  Engine::Renderer::beginScene();

  // PLANETA
  mPlanetShader->bind();
  mCameraController.updateShader(mPlanetShader);
  mLight->update(mPlanetShader);

  mEarth.onUpdate(dt);
  mEarth.draw(mPlanetShader);

  // NUBES
  glDepthMask(GL_FALSE);
  mCloudShader->bind();
  mCameraController.updateShader(mCloudShader);
  mLight->update(mCloudShader);

  mEarthClouds.onUpdate(dt);
  mEarthClouds.drawCubeSphere(mCloudShader);
  glDepthMask(GL_TRUE);

  glDepthFunc(GL_LEQUAL);
  mCameraController.updateShader(mSkyboxShader);
  mSkyboxShader->bind();
  mSkyboxShader->setMat4("projection", mCameraController.getCamera().getProjection());
  mSkyboxShader->setMat4("view", glm::mat4(glm::mat3(mCameraController.getCamera().getViewMatrix())));
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

  if (viewportSize.x > 0 && viewportSize.y > 0)
    mViewportSize = {viewportSize.x, viewportSize.y};

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
  mCameraController.onEvent(e);

  Engine::EventDispatcher dispatcher(e);
  dispatcher.dispatch<Engine::KeyPressedEvent>(BIND_FN(ApplicationLayer::onKeyPressedEvent));
}

bool ApplicationLayer::onKeyPressedEvent(Engine::KeyPressedEvent &event) {
  if (Engine::Input::isKeyPressed(Engine::Key::Escape)) {
    onClose();
  }
  return false;
}

bool ApplicationLayer::onMouseMoved(Engine::MouseMovedEvent &event) {
  return false;
}

bool ApplicationLayer::onWindowResize(WindowResizeEvent &event) {
  // mCameraController.getCamera().setAspect((float)event.getWidth() / (float)event.getHeight());

  return false;
}

void ApplicationLayer::onClose() {
  // this event only triggers when the key event ocurred with in the app layer
  Engine::WindowCloseEvent e;
  Engine::Application::get().onEvent(e);
}
