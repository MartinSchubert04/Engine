#pragma once
#include "pch.h"

namespace Engine {

struct FrameBufferSpecification {

  uint32_t width, height;
  uint32_t samples = 1;

  bool swapChainTarget = false;
};

class FrameBuffer {

public:
  virtual void bind() const = 0;
  virtual void unbind() const = 0;
  virtual const FrameBufferSpecification &getSpecification() const = 0;

  static Ref<FrameBuffer> create(FrameBufferSpecification &spec);
};

}  // namespace Engine
