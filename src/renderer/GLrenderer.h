#pragma once

#include "Render.h"
#include "window/Window.h"
#include "elements/Shader.h"

namespace render {

class GLrenderer : public render::RenderContext {
public:
  bool init(window::Iwindow *window) override;

  void preRender() override;

  void postRender() override;

  void end() override;
};

}  // namespace render
