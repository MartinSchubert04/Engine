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

  SetDarkThemeColors();

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

void ImGuiLayer::SetDarkThemeColors() {
  auto &colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

}  // namespace Engine
