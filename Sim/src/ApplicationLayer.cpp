#include "ApplicationLayer.h"
#include "Core/DeltaTime.h"
#include "Core/Log.h"
#include "Planet.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "pch.h"
#include <vector>

ApplicationLayer::ApplicationLayer() : Layer("App layer") {

  // vertexArray = Engine::VertexArray::create();
  // std::vector<glm::vec3> positions = {{-0.5f, -0.5f, 0.0f}, {0.5f, -0.5, 0.0f}, {0.0f, 0.5f, 0.0f}};
  // std::vector<glm::vec4> colors = {{1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}};
  // std::vector<Vertex> vertices;

  // for (unsigned int i{0}; i < 3; i++) {
  //   Vertex v;
  //   v.position = positions[i];
  //   v.color = colors[i];

  //   vertices.push_back(v);
  // }

  // vertexBuffer = Engine::VertexBuffer::create(vertices);

  // // fill layout with buffer elements then gets added to vertex array
  // Engine::BufferLayout layout = {
  //     {Engine::Types::ShaderDataType::float3, "a_Pos"},
  //     {Engine::Types::ShaderDataType::float4, "a_Color"},
  // };

  // vertexBuffer->setLayout(layout);
  // vertexArray->addVertexBuffer(vertexBuffer);

  // std::vector<uint32_t> indices = {0, 1, 2};
  // indexBuffer = Engine::IndexBuffer::create(indices);

  // vertexArray->setIndexBuffer(indexBuffer);

  //------------------------------------------------------------------------------------------------------------------------

  mSkybox.loadCubeMap(std::vector<std::string>({"Sim/Assets/textures/skybox.png", "Sim/Assets/textures/skybox.png",
                                                "Sim/Assets/textures/skybox.png", "Sim/Assets/textures/skybox.png",
                                                "Sim/Assets/textures/skybox.png", "Sim/Assets/textures/skybox.png"}));

  mSkyboxVAO = Engine::VertexArray::create();
  Engine::Ref<Engine::VertexBuffer> skyboxVBO = Engine::VertexBuffer::create(mSkybox.getVertices());
  Engine::Ref<Engine::IndexBuffer> skyboxIBO = Engine::IndexBuffer::create(mSkybox.getIndices());

  Engine::BufferLayout skyboxLayout = {{Engine::Types::ShaderDataType::float3, "aPos"}};

  skyboxVBO->setLayout(skyboxLayout);
  mSkyboxVAO->addVertexBuffer(skyboxVBO);
  mSkyboxVAO->setIndexBuffer(skyboxIBO);

  mShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/triangle.vs", "Sim/Assets/shaders/triangle.fs");
  mSkyboxShader = Engine::createScope<Engine::Shader>("Sim/Assets/shaders/skybox.vs", "Sim/Assets/shaders/skybox.fs");
  mCamera = Engine::createScope<Engine::Camera>(glm::vec3(0, 0, 1), 45.0f, 1.3f, 0.1f, 1000);
}

void ApplicationLayer::onUpdate(Engine::DeltaTime dt) {
  mDeltaTime = dt;

  Engine::RenderCommand::setClearColor({.2, .2, .2, 1});
  Engine::RenderCommand::clear();

  Engine::Renderer::beginScene();
  glEnable(GL_DEPTH_TEST);

  mCamera->update(mShader.get());
  mCamera->update(mSkyboxShader.get());

  mShader->bind();

  // Planet sphere(1, glm::vec2(12, 12), glm::vec3(0, 0, 0), 1);
  // sphere.draw(mShader);

  // Engine::Transform transform;
  // transform.setModel(mShader);

  glDepthMask(GL_FALSE);
  mSkyboxShader->bind();
  mSkyboxVAO->bind();
  mSkybox.bind();
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glDepthMask(GL_TRUE);

  // Engine::Renderer::submit(vertexArray);

  Engine::Renderer::endScene();
}

void ApplicationLayer::onEvent(Engine::Event &e) {

  Engine::EventDispatcher dispatcher(e);

  dispatcher.dispatch<Engine::KeyPressedEvent>(BIND_FN(ApplicationLayer::onKeyPressedEvent));
  dispatcher.dispatch<Engine::MouseMovedEvent>(BIND_FN(ApplicationLayer::onMouseMoved));
}

bool ApplicationLayer::onKeyPressedEvent(Engine::KeyPressedEvent &event) {
  if (event.getKeyCode() == Engine::Key::Escape) {
    close();
  }
  if (event.getKeyCode() == Engine::Key::W) {
    mCamera->onMouseWheel(-mCamera->speed * mDeltaTime.getSeconds());
  }
  if (event.getKeyCode() == Engine::Key::S) {
    mCamera->onMouseWheel(mCamera->speed * mDeltaTime.getSeconds());
  }

  return false;
}

bool ApplicationLayer::onMouseMoved(Engine::MouseMovedEvent &event) {
  if (Engine::Input::isMouseButtonPressed(Engine::Mouse::ButtonRight)) {
    mCamera->onMouseMove(event.getX(), event.getY(), Engine::Mouse::ButtonRight);
  }

  return false;
}

void ApplicationLayer::close() {
  // this event only triggers
  Engine::WindowCloseEvent e;
  Engine::Application::get().onEvent(e);
}
