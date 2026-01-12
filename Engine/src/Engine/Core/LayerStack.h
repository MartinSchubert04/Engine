#include "Layer.h"

namespace Engine {

class LayerStack {

public:
  LayerStack() = default;
  ~LayerStack();

  void pushLayer(Layer *layer);
  void pushOverlay(Layer *overlay);
  void popLayer(Layer *layer);
  void popOverlay(Layer *overlay);

  inline std::vector<Layer *>::iterator begin() { return mLayers.begin(); }
  inline std::vector<Layer *>::iterator end() { return mLayers.end(); }
  inline std::vector<Layer *>::reverse_iterator rbegin() { return mLayers.rbegin(); }
  inline std::vector<Layer *>::reverse_iterator rend() { return mLayers.rend(); }

  inline std::vector<Layer *>::const_iterator begin() const { return mLayers.begin(); }
  inline std::vector<Layer *>::const_iterator end() const { return mLayers.end(); }
  inline std::vector<Layer *>::const_reverse_iterator rbegin() const { return mLayers.rbegin(); }
  inline std::vector<Layer *>::const_reverse_iterator rend() const { return mLayers.rend(); }

private:
  std::vector<Layer *> mLayers;
  unsigned int mLayerInsertIndex = 0;
};

}  // namespace Engine
