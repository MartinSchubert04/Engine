#pragma once
#include "Render.h"

namespace render {

class Interface : public RenderContext {

public:
  ~Interface();

  bool init(window::Iwindow *window) override;

  void preRender() override;

  void postRender() override;

  void end() override;
};
}  // namespace render
