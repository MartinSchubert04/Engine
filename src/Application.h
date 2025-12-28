#pragma once

#include <string>
#include "window/GLwindow.h"
#include <memory>

namespace Core {
class Application {

public:
  void run();
  Application(const std::string &name);

private:
  std::unique_ptr<window::GLwindow> mWindow;
};

}  // namespace Core
