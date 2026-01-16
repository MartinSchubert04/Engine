#include "ImGuiLayer.h"
#include "Core/Application.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace Engine {

ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::onAttach() {

  ImGui::CreateContext();

  // setup theme
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  ImGuiIO &io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

  float fontSize = 18.0f;  // *2.0f;
  // io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans/OpenSans-Bold.ttf", fontSize);
  // io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans/OpenSans-Regular.ttf", fontSize);

  Application &app = Application::get();

  GLFWwindow *window = static_cast<GLFWwindow *>(app.getWindow().getNativeWindow());

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");
}

void ImGuiLayer::onDetach() {
  ImGui::DestroyPlatformWindows();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::onUpdate() {

  ImGuiIO &io = ImGui::GetIO();

  Window &window = Application::get().getWindow();
  io.DisplaySize = ImVec2(window.getWidth(), window.getHeight());

  float time = (float)glfwGetTime();
  io.DeltaTime = mTime > 0 ? (time - mTime) : (1.0f / 60.0f);
  mTime = time;

  ImGui_ImplOpenGL3_NewFrame();  // render
  ImGui_ImplGlfw_NewFrame();  // plataforma
  ImGui::NewFrame();  // gen new frame

  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::onEvent(Event &event) {}

}  // namespace Engine
