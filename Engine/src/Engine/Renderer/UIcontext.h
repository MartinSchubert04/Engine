#pragma once
#include "Render.h"

namespace render {

class UIcontext : public RenderContext {

public:
  bool init(window::Iwindow *window) override;

  void preRender() override;

  void postRender() override;

  void end() override;
};
}  // namespace render
