#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "Core/Base.h"
#include "Core/Log.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Events/KeyEVent.h"
#include "Events/MouseEVent.h"
#include "imgui.h"
#include "pch.h"

namespace Engine {

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::onAttach() {

  ImGui::CreateContext();

  // setup theme
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows

  // viewports set up
  // io.ConfigFlags |= ImGuiViewportFlags_NoTaskBarIcon;
  // io.ConfigFlags |= ImGuiViewportFlags_NoAutoMerge;

  float fontSize = 18.0f;  // *2.0f;
  io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/OpenSans/OpenSans-Bold.ttf", fontSize);
  io.FontDefault = io.Fonts->AddFontFromFileTTF("Editor/Assets/Fonts/OpenSans/OpenSans-Regular.ttf", fontSize);

  ImGuiStyle &style = ImGui::GetStyle();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  Application &app = Application::get();

  GLFWwindow *window = static_cast<GLFWwindow *>(app.getWindow().getNativeWindow());

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");
}

void ImGuiLayer::onDetach() {
  ImGui::DestroyPlatformWindows();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event &e) {

  if (mBlockEvents) {
    ImGuiIO &io = ImGui::GetIO();
    e.handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
    e.handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
  }
}

void ImGuiLayer::begin() {
  ImGuiIO &io = ImGui::GetIO();
  float time = (float)glfwGetTime();
  io.DeltaTime = mTime > 0 ? (time - mTime) : (1.0f / 60.0f);
  mTime = time;

  ImGui_ImplOpenGL3_NewFrame();  // render
  ImGui_ImplGlfw_NewFrame();  // plataforma
  ImGui::NewFrame();  // gen new frame
}

void ImGuiLayer::end() {
  ImGuiIO &io = ImGui::GetIO();

  Window &window = Application::get().getWindow();
  io.DisplaySize = ImVec2(window.getWidth(), window.getHeight());

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::onImGuiRender() {
  ImGuiIO &io = ImGui::GetIO();

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
}

}  // namespace Engine
