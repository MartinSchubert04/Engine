#include "Panel.h"
#include "utils/imgui_widgets.h"

namespace UI {

void Panel::render(UI::Scene *scene) {

  auto model = scene->getModel();

  ImGui::Begin("Properties");
  if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {

    if (ImGui::Button("Open...")) {
      mFileDialog.Open();
    }
    ImGui::SameLine(0, 5.0f);
    ImGui::Text("%s", mCurrentFile.c_str());
  }

  if (ImGui::CollapsingHeader("Material") && model) {
    ImGui::ColorPicker3("Color", (float *)&model->color,
                        ImGuiColorEditFlags_PickerHueWheel |
                            ImGuiColorEditFlags_DisplayRGB);
    ImGui::SliderFloat("Roughness", &model->roughness, 0.0f, 1.0f);
    ImGui::SliderFloat("Metallic", &model->metallic, 0.0f, 1.0f);
  }

  if (ImGui::CollapsingHeader("Light")) {

    ImGui::Separator();
    ImGui::Text("Position");
    ImGui::Separator();
    utils::draw_vec3_widget("Position", scene->getLight()->position, 80.0f);
  }

  ImGui::End();

  mConsole.draw();

  mFileDialog.Display();
  if (mFileDialog.HasSelected()) {
    std::filesystem::path selectedPath = mFileDialog.GetSelected();

    std::string file_path = selectedPath.generic_string();

    mCurrentFile = selectedPath.filename().string();

    mModelLoadCallback(file_path);

    mFileDialog.ClearSelected();
  }
}

}  // namespace UI
