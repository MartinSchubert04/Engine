#include "Application.h"

int main() {
  auto app = std::make_unique<Core::Application>("Engine");

  app->run();
}
