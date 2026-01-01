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

  mFileDialog.Display();
  if (mFileDialog.HasSelected()) {
    auto file_path = mFileDialog.GetSelected().string();
    mCurrentFile = file_path.substr(file_path.find_last_of("/\\") + 1);

    mModelLoadCallback(file_path);

    mFileDialog.ClearSelected();
  }
}

}  // namespace UI
