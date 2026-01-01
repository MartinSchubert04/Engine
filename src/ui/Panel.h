#include "common.h"
#include "Scene.h"

namespace UI {

class Panel {
private:
  // create a file browser instance
  ImGui::FileBrowser mFileDialog;

  std::function<void(const std::string &)> mModelLoadCallback;

  std::string mCurrentFile;

public:
  Panel() {
    mCurrentFile = "<...>";

    mFileDialog.SetTitle("Open model");
    mFileDialog.SetTypeFilters({".fbx", ".obj"});
  }

  void render(UI::Scene *scene);

  void setModel_load_callback(
      const std::function<void(const std::string &)> &callback) {
    mModelLoadCallback = callback;
  }
};

}  // namespace UI
