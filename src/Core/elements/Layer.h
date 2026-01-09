#include "common.h"

namespace Engine {

class Layer {
protected:
  const std::string mDebugName;

public:
  Layer(const std::string &debugName = "Layer");

  virtual ~Layer() = default;

  virtual void onAttach() {};
  virtual void onDetach() {};
  virtual void onUpdate(double dt) {};
  // requieres event system implementation
  // virtual void  OnEvent(Event& event) {};

  inline const std::string &getName() { return mDebugName; }
};

}  // namespace Engine
