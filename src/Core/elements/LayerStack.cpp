#include "LayerStack.h"

namespace Engine {

LayerStack::~LayerStack() {
  for (auto layer : mLayers)
    delete layer;
}

void LayerStack::pushLayer(Layer *layer) {
  mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
  mLayerInsertIndex++;
}

void LayerStack::popLayer(Layer *layer) {

  // search in layers portion
  auto it = std::find(mLayers.begin(), mLayers.begin() + mLayerInsertIndex, layer);
  if (it != mLayers.begin() + mLayerInsertIndex) {
    layer->onDetach();
    mLayers.erase(it);
    mLayerInsertIndex--;
  }
}

void LayerStack::pushOverlay(Layer *overlay) {
  mLayers.emplace_back(overlay);
}

void LayerStack::popOverlay(Layer *overlay) {

  // search in overlay location (last portion of vector)
  auto it = std::find(mLayers.begin() + mLayerInsertIndex, mLayers.end(), overlay);
  if (it != mLayers.end()) {
    overlay->onAttach();
    mLayers.erase(it);
  }
}

}  // namespace Engine
