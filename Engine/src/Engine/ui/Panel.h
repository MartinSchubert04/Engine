#include "pch.h"
#include "Scene.h"
#include "Console.h"
#include <filesystem>

namespace UI {

class Panel {
private:
  // create a file browser instance
  ImGui::FileBrowser mFileDialog;

  Console mConsole;
  std::unique_ptr<LogRedirector> mCoutRedirector;
  std::unique_ptr<LogRedirector> mCerrRedirector;

  std::function<void(const std::string &)> mModelLoadCallback;

  std::string mCurrentFile;

public:
  Panel() {
    mCurrentFile = "<...>";

    // generic_string() uses '/' for paths
    std::filesystem::path currentDir = std::filesystem::current_path().generic_string();

    mFileDialog.SetDirectory(currentDir.string() + "/resources/model");

    mFileDialog.SetTitle("Open model");
    mFileDialog.SetTypeFilters({".fbx", ".obj"});

    mCoutRedirector = std::make_unique<LogRedirector>(std::cout, &mConsole);
    mCerrRedirector = std::make_unique<LogRedirector>(std::cerr, &mConsole);
  }

  void render(UI::Scene *scene);

  void setModel_load_callback(const std::function<void(const std::string &)> &callback) {
    mModelLoadCallback = callback;
  }
};

}  // namespace UI
