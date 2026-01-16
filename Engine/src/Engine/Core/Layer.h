#pragma once

#include "pch.h"
#include "Events/Event.h"
namespace Engine {

class Layer {
protected:
  const std::string mDebugName;

public:
  Layer(const std::string &debugName = "Layer");

  virtual ~Layer() = default;

  virtual void onAttach() {};
  virtual void onDetach() {};
  virtual void onUpdate() {};
  virtual void onEvent(Event &event) {};

  inline const std::string &getName() { return mDebugName; }
};

}  // namespace Engine
